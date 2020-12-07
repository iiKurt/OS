/**
 * Thanks to https://wiki.osdev.org/User:No92/UEFI_Bare_Bones for the basic "Hello World" code,
 * some parts of the makefile, and a link to the Zircon EFI headers
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <efi/efi.h>
#include <efi/statuses.h>
#include <elf.h>

#include "services/LoadFile.h"

typedef unsigned long long size_t;
typedef uint64_t UINTN;

typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;



Framebuffer framebuffer;
Framebuffer* InitializeGOP(EFI_SYSTEM_TABLE* SystemTable){
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;

	status = SystemTable->BootServices->LocateProtocol(&gopGuid, NULL, (void**)&gop);
	if(status == EFI_ERROR){
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Unable to locate GOP\n\r");
		return NULL;
	}
	else
	{
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"GOP located\n\r");
	}

	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &framebuffer;
	
}

int memcmp(const void* aptr, const void* bptr, size_t n){
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++){
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Loading kernel...\n\r");

	// Acquire kernel image
	EFI_FILE_PROTOCOL* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);
	if (Kernel == NULL){
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel not acquired\n\r");

		EFI_INPUT_KEY key;
		while(SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) == EFI_NOT_READY); // freeze the system
		
		return EFI_LOAD_ERROR;
	}
	else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel acquired\n\r");
	}

	// Check kernel image format
	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;

		Kernel->GetInfo(Kernel, &EFI_FILE_INFO_GUID, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &EFI_FILE_INFO_GUID, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	}

	if (
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	)
	{
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel format is bad\r\n");
	}
	else
	{
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel header successfully verified\r\n");
	}

	// Read the kernel into memory
	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for (
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	)
	{
		switch (phdr->p_type){
			case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void*)segment);
				break;
			}
		}
	}

	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel loaded\n\r");

	Framebuffer* fb = InitializeGOP(SystemTable);

	unsigned int y = 50;
	unsigned int BBP = 4; //4 bytes per pixel

	for (unsigned int x = 0; x < fb->Width / 2 * BBP; x++) {
		*(unsigned int*)(x + (y * fb->PixelsPerScanLine * BBP) + fb->BaseAddress) = 0xff0000ff;
	}

	// Call kernel
	((__attribute__((sysv_abi)) void (*)())header.e_entry)();
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Wait so does it work up until here?!\n\r");

	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	// we _shouldn't_ ever reach this point
	return EFI_SUCCESS; // Exit the UEFI application (may crash the system after the kernel is loaded)
}
