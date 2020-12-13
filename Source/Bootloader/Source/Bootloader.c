/**
 * Thanks to https://wiki.osdev.org/User:No92/UEFI_Bare_Bones for the basic "Hello World" code,
 * some parts of the makefile, and a link to the Zircon EFI headers
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <Framebuffer.h>
#include <PSF.h>
#include <BootInfo.h>

#include "EFI/EFI.h"
#include "EFI/Statuses.h"
#include "ELF.h"

#include "Services/LoadFile.h"
#include "Services/LoadKernel.h"
#include "Services/GOP.h"
#include "Services/LoadFont.h"

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
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel not found\n\r");

		pause(SystemTable);
		return EFI_LOAD_ERROR;
	}
	else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel found\n\r");
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
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel read\n\r");
	}
	else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel not read\n\r");
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

	// Do the PSF1 font
	PSF1_FONT* font = LoadPSF1Font(NULL, L"zap-light16.psf", ImageHandle, SystemTable);

	if (font == NULL) { // Font not found
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Font not found or is not valid\n\r");
		pause(SystemTable);
		return EFI_ERROR;
	}
	else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Font found\n\r");
	}

	// Memory
	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapSize, MapKey;
	UINTN DescriptorSize;
	uint32_t DescriptorVersion;
	{
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}

	BootInfo bootInfo;
	bootInfo.fb = fb;
	bootInfo.font = font;
	bootInfo.mMap = Map;
	bootInfo.mMapSize = MapSize;
	bootInfo.mMapDescriptorSize = DescriptorSize;
	
	// Disable the watchdog timer just in case
	SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
	// System will crash after this point if we use any EFI services
	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

	// Call kernel
	((__attribute__((sysv_abi)) void (*)(BootInfo*))header.e_entry)(&bootInfo);

	// we _shouldn't_ ever reach this point
	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS; // Exit the UEFI application (may crash the system after the kernel is loaded)
}
