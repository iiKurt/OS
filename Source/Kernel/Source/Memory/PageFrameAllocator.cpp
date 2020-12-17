#include "PageFrameAllocator.hpp"

#include <stddef.h>

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
	LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);

	// Reserve pages of unusable/reversed memory
	for (unsigned int i = 0; i < mMapEntries; i++) {
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescriptorSize));

		if (desc->type != 7) { // Is not EfiConventionalMemory
			// Reserve it
			ReservePages(desc->physicalAddress, desc->pageCount);
		}
	}
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress) {
	PageBitmap.Size = bitmapSize;
	PageBitmap.Buffer = (uint8_t*)bufferAddress;

	for (unsigned int i = 0; i < bitmapSize; i++)
	{
		*(uint8_t*)(PageBitmap.Buffer + i) = 0;
	}
}


void* PageFrameAllocator::RequestPage() {
	for (uint64_t i = 0; i < PageBitmap.Size * 8; i++) {
		if (PageBitmap[i] == true) {
			continue;
		}

		LockPage((void*)(i * 4096));
		return (void*)(i * 4096);
	}

	return NULL; // Oopies! Ideally we want to perform a disk page file swap here
}


void PageFrameAllocator::FreePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (PageBitmap[index] == false) { // Already free
		return;
	}
	PageBitmap.Set(index, false); // Make it free

	freeMemory += 4096;
	usedMemory -= 4096;
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount) {
	for (unsigned int i = 0; i < pageCount; i++) {
		FreePage((void*)((uint64_t)address + (i * 4096)));
	}
}

void PageFrameAllocator::LockPage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (PageBitmap[index] == true) { // Already locked
		return;
	}
	PageBitmap.Set(index, true); // Make it locked

	freeMemory -= 4096;
	usedMemory += 4096;
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount) {
	for (unsigned int i = 0; i < pageCount; i++) {
		LockPage((void*)((uint64_t)address + (i * 4096)));
	}
}

void PageFrameAllocator::UnreservePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (PageBitmap[index] == false) { // Already unreserved
		return;
	}
	PageBitmap.Set(index, false); // Make it unreserved

	freeMemory += 4096;
	reservedMemory -= 4096;
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount) {
	for (unsigned int i = 0; i < pageCount; i++) {
		UnreservePage((void*)((uint64_t)address + (i * 4096)));
	}
}

void PageFrameAllocator::ReservePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (PageBitmap[index] == true) { // Already reserved
		return;
	}
	PageBitmap.Set(index, true); // Make it rerserved

	freeMemory -= 4096;
	reservedMemory += 4096;
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount) {
	for (unsigned int i = 0; i < pageCount; i++) {
		ReservePage((void*)((uint64_t)address + (i * 4096)));
	}
}


uint64_t PageFrameAllocator::GetFreeRAM() {
	return freeMemory;
}

uint64_t PageFrameAllocator::GetUsedRAM() {
	return usedMemory;
}

uint64_t PageFrameAllocator::GetReservedRAM() {
	return reservedMemory;
}
