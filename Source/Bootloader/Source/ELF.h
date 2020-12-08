// https://github.com/rv8-io/rv8/blob/master/src/elf/elf.h

#pragma once

#define	ELFMAG		"\177ELF"
#define	SELFMAG		4

typedef unsigned char      _elf_u8;
typedef unsigned short int _elf_u16;
typedef signed int         _elf_s32;
typedef unsigned int       _elf_u32;
typedef signed long long   _elf_s64;
typedef unsigned long long _elf_u64;

// Elf64 types
typedef _elf_u64 Elf64_Addr;
typedef _elf_u16 Elf64_Half;
typedef _elf_u64 Elf64_Off;
typedef _elf_s32 Elf64_Sword;
typedef _elf_u32 Elf64_Word;
typedef _elf_u64 Elf64_Xword;
typedef _elf_s64 Elf64_Sxword;
typedef _elf_u8  Elf64_Byte;

// e_ident
enum {
	EI_MAG0 = 0,                     /* File identification */
	EI_MAG1 = 1,
	EI_MAG2 = 2,
	EI_MAG3 = 3,
	EI_CLASS = 4,                    /* File class */ 
	EI_DATA = 5,                     /* Data encoding */ 
	EI_VERSION = 6,                  /* File version */ 
	EI_OSABI = 7,                    /* OS/ABI identification */
	EI_ABIVERSION = 8,               /* ABI version */
	EI_PAD = 9,                      /* Start of padding bytes */
	EI_NIDENT = 16                   /* Size of e_ident[] */
};

// EI_CLASS
enum {
	ELFCLASSNONE = 0,
	ELFCLASS32 = 1,                  /* e_ident[EI_CLASS] - 32-bit objects */
	ELFCLASS64 = 2,                  /* e_ident[EI_CLASS] - 64-bit objects */
	ELFCLASS128 = 3                  /* e_ident[EI_CLASS] - 128-bit objects */
};

// EI_DATA
enum {
	ELFDATANONE = 0,
	ELFDATA2LSB = 1,                 /* e_ident[EI_DATA] - Data is little-endian */
	ELFDATA2MSB = 2                  /* e_ident[EI_DATA] - Data is big-endian */
};

// ELF Extension Header

// e_type
enum {
	ET_NONE = 0,                     /* No file type */
	ET_REL = 1,                      /* Relocatable file */
	ET_EXEC = 2,                     /* Executable file */
	ET_DYN = 3,                      /* Shared object file */
	ET_CORE = 4,                     /* Core file */
	ET_LOPROC = 0xff00,              /* Processor-specific */
	ET_HIPROC = 0xffff
};

// e_machine
enum {
	EM_NONE = 0,                     /* Unknown machine */
	EM_X86_64 = 62,                  /* x86-64 */
};

// e_version
enum {
	EV_NONE = 0,                     /* Invalid version */
	EV_CURRENT = 1                   /* Current version */
};

// Elf64_Ehdr
typedef struct {
	Elf64_Byte  e_ident[EI_NIDENT];  /* ELF identification */
	Elf64_Half  e_type;              /* Object file type */
	Elf64_Half  e_machine;           /* Machine type */
	Elf64_Word  e_version;           /* Object file version */
	Elf64_Addr  e_entry;             /* Entry point address */
	Elf64_Off   e_phoff;             /* Program header offset */
	Elf64_Off   e_shoff;             /* Section header offset */
	Elf64_Word  e_flags;             /* Processor-specific flags */
	Elf64_Half  e_ehsize;            /* ELF header size */
	Elf64_Half  e_phentsize;         /* Size of program header entry */
	Elf64_Half  e_phnum;             /* Number of program header entries */
	Elf64_Half  e_shentsize;         /* Size of section header entry */
	Elf64_Half  e_shnum;             /* Number of section header entries */
	Elf64_Half  e_shstrndx;          /* Section name string table index */
} Elf64_Ehdr;

// p_type
enum {
	PT_NULL = 0,                     /* Unused entry */
	PT_LOAD = 1,                     /* Loadable segment */
	PT_DYNAMIC = 2,                  /* Dynamic linking tables */
	PT_INTERP = 3,                   /* Program interpreter path name */
	PT_NOTE = 4,                     /* Note sections */ 
	PT_SHLIB = 5,                    /* Reserved */
	PT_PHDR = 6,                     /* Program header table */
	PT_TLS = 7,                      /* Thread local storage header table */
	PT_LOOS = 0x60000000,            /* Environment-specific use */
	PT_GNU_EH_FRAME = 0x6474e550,    /* GCC .eh_frame_hdr segment */
	PT_GNU_STACK = 0x6474e551,       /* GCC stack executability */
	PT_GNU_RELRO = 0x6474e552,       /* GCC Read-only after relocation */
	PT_HIOS = 0x6fffffff,
	PT_LOPROC = 0x70000000,          /* Processor-specific use */
	PT_HIPROC = 0x7fffffff
};

// Elf64_Phdr
typedef struct {
	Elf64_Word  p_type;              /* Type of segment */
	Elf64_Word  p_flags;             /* Segment attributes */
	Elf64_Off   p_offset;            /* Offset in file */
	Elf64_Addr  p_vaddr;             /* Virtual address in memory */
	Elf64_Addr  p_paddr;             /* Physical address in memory (if applicable) */
	Elf64_Xword p_filesz;            /* Size of segment in file */
	Elf64_Xword p_memsz;             /* Size of segment in memory */
	Elf64_Xword p_align;             /* Alignment of segment */
} Elf64_Phdr;
