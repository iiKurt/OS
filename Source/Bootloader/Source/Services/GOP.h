#pragma once

#include <Framebuffer.h>

#include "../EFI/EFI.h"

Framebuffer* InitializeGOP(EFI_SYSTEM_TABLE* SystemTable);
