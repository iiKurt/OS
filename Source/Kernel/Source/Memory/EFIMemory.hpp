#pragma once

#include <stdint.h>

struct EFI_MEMORY_DESCRIPTOR {
	uint32_t type; // Type
	void* physicalAddress; // PhysicalStart
	void* virtualAddress; // VirtualStart
	uint64_t pageCount; // NumberOfPages
	uint64_t attributes; // Attribute
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];
