#pragma once

#include "../EFI/EFI.h"
#include "../ELF.h"

void ReadKernel(EFI_SYSTEM_TABLE* SystemTable, EFI_FILE_PROTOCOL* Kernel, Elf64_Ehdr* header);
int KernelValid(Elf64_Ehdr* header);
int LoadKernel(EFI_SYSTEM_TABLE* SystemTable, EFI_FILE_PROTOCOL* Kernel, Elf64_Ehdr* header);
