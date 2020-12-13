#include "Memory.hpp"

uint64_t GetMemorySize(void* mMap, uint64_t mMapEntries, uint64_t mMapDescriptorSize) {
	static uint64_t memorySizeBytes = 0;
	if (memorySizeBytes > 0) { // Already been calculated
		return memorySizeBytes; // Returned cached size
	}

	for (unsigned int i = 0; i < mMapEntries; i++) {
		// Get the descriptor at an offset to the memory map
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescriptorSize));
		// pageCount is the number of 4KiB pages
		// Calculate how many KiBs by multiplying by 4
		// Calculate the number of bytes by multiplying by the amount of bytes in 1 4KiB page (4096)
		memorySizeBytes += desc->pageCount * 4096;
	}

	return memorySizeBytes;
}
