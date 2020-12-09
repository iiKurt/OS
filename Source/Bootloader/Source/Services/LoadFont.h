#pragma once

#include <PSF.h>
#include "../EFI/EFI.h"

PSF1_FONT* LoadPSF1Font(EFI_FILE_PROTOCOL* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
