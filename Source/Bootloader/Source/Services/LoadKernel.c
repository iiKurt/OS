#include "LoadKernel.h"

#include <stddef.h>

void ReadKernel(EFI_SYSTEM_TABLE* SystemTable, EFI_FILE_PROTOCOL* Kernel, Elf64_Ehdr* header) {
	UINTN FileInfoSize;
	EFI_FILE_INFO* FileInfo;

	Kernel->GetInfo(Kernel, &EFI_FILE_INFO_GUID, &FileInfoSize, NULL);
	SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
	//Kernel->GetInfo(Kernel, &EFI_FILE_INFO_GUID, &FileInfoSize, (void**)&FileInfo); // << BAD

	UINTN size = sizeof(*header);
	Kernel->Read(Kernel, &size, header);
	
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Function done\n\r");
}

int memcmp(const void* aptr, const void* bptr, size_t n){
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++){
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

int KernelValid(Elf64_Ehdr* header) {
	return	memcmp(&header->e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
			header->e_ident[EI_CLASS] != ELFCLASS64 ||
			header->e_ident[EI_DATA] != ELFDATA2LSB ||
			header->e_type != ET_EXEC ||
			header->e_machine != EM_X86_64 ||
			header->e_version != EV_CURRENT;
}

int LoadKernel(EFI_SYSTEM_TABLE* SystemTable, EFI_FILE_PROTOCOL* Kernel, Elf64_Ehdr* header) {
	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header->e_phoff);
		UINTN size = header->e_phnum * header->e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for (
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header->e_phnum * header->e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header->e_phentsize)
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
				return 1;
			}
		}
	}
	return 0;
}
