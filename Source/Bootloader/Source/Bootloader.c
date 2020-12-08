/**
 * Thanks to https://wiki.osdev.org/User:No92/UEFI_Bare_Bones for the basic "Hello World" code,
 * some parts of the makefile, and a link to the Zircon EFI headers
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "EFI/EFI.h"
#include "EFI/Statuses.h"
#include "ELF.h"

#include "Services/LoadFile.h"
#include "Services/LoadKernel.h"
#include "Services/GOP.h"

void pause(EFI_SYSTEM_TABLE* SystemTable) {
	EFI_INPUT_KEY key;
	while(SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) == EFI_NOT_READY); // freeze the system until keypress
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Loading kernel...\n\r");

	// Acquire kernel image
	EFI_FILE_PROTOCOL* Kernel = LoadFile(NULL, L"Kernel.elf", ImageHandle, SystemTable);
	if (Kernel == NULL){
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel not acquired\n\r");

		pause(SystemTable);
		return EFI_LOAD_ERROR;
	}
	else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel acquired\n\r");
	}

	// Read kernel image into header
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

	// Check if kernel image format is valid
	if (KernelValid(&header))
	{
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel format is bad\r\n");
		pause(SystemTable);
		return EFI_LOAD_ERROR;
	}
	else
	{
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel header successfully verified\r\n");
	}

	// Read the kernel into memory
	if (ReadKernel(SystemTable, Kernel, &header)) {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel loaded\n\r");
	}
	else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel not loaded\n\r");
		pause(SystemTable);
		return EFI_LOAD_ERROR;
	}
	
	// Do the GOP
	Framebuffer* fb = InitializeGOP(SystemTable);

	if (fb == NULL) {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"GOP not found\n\r");
		pause(SystemTable);
		return EFI_ERROR;
	}
	else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"GOP located\n\r");
	}

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
