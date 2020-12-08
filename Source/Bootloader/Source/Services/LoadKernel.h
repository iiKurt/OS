#pragma once

#include "../EFI/EFI.h"
#include "../ELF.h"

int KernelValid(Elf64_Ehdr* header);
int ReadKernel(EFI_SYSTEM_TABLE* SystemTable, EFI_FILE_PROTOCOL* Kernel, Elf64_Ehdr* header);
