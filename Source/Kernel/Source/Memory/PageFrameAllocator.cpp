#include "PageFrameAllocator.hpp"

uint64_t freeMemory; // Unused memory
uint64_t reservedMemory; // Reserved/unusable memory for e.g. ACPI tables
uint64_t usedMemory; // Allocated memory from the page frame allocator
bool Initialised = false;

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescriptorSize) {
	if (Initialised) { // Don't initialise twice
		return;
	}

	uint64_t mMapEntries = mMapSize / mMapDescriptorSize;

	void* largestFreeMemSeg = NULL; // Size in bytes (* 4096)
	size_t largestFreeMemSegSize = 0;

	for (unsigned int i = 0; i < mMapEntries; i++) {
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescriptorSize));

		if (desc->type == 7) { // 7 is EfiConventionalMemory
			if (desc->pageCount * 4096 > largestFreeMemSegSize) {
				largestFreeMemSeg = desc->physicalAddress;
				largestFreeMemSegSize = desc->pageCount;
			}
		}
	}

	uint64_t memorySize = GetMemorySize(mMap, mMapEntries, mMapDescriptorSize);
	freeMemory = memorySize;

	uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

	// Init the bitmap
	InitBitmap(bitmapSize, largestFreeMemSeg);

	// Lock pages of bitmap
	// Reserve pages of unusable/reversed memory
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress) {
	PageBitmap.Size = bitmapSize;
	PageBitmap.Buffer = (uint8_t*)bufferAddress;

	for (unsigned int i = 0; i < bitmapSize; i++)
	{
		*(uint8_t*)(PageBitmap.Buffer + i) = 0;
	}
}
