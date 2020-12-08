#pragma once

#include "../EFI/EFI.h"
#include "../EFI/Statuses.h"

EFI_FILE_PROTOCOL* LoadFile(EFI_FILE_PROTOCOL* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
