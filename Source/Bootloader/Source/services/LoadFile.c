#include "LoadFile.h"

#include <stddef.h>

EFI_FILE_PROTOCOL* LoadFile(EFI_FILE_PROTOCOL* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE_PROTOCOL* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	//efi_guid loaded_image_protocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &EFI_LOADED_IMAGE_PROTOCOL_GUID, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (void**)&FileSystem);
	
	if (Directory == NULL){
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS){
		return NULL;
	}
	return LoadedFile;
}
