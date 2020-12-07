#pragma once

#include <efi/efi.h>
#include <efi/statuses.h>

EFI_FILE_PROTOCOL* LoadFile(EFI_FILE_PROTOCOL* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
