#pragma once

#include <stdint.h>

#include "../Memory.hpp"
#include "../EFIMemory.hpp"
#include "../../Bitmap.hpp"

class PageFrameAllocator {
private:
	void InitBitmap(size_t bitMapSize, void* bufferAddress);
	void UnreservePage(void* address);
	void UnreservePages(void* address, uint64_t pageCount);
	void ReservePage(void* address);
	void ReservePages(void* address, uint64_t pageCount);

public:
	Bitmap PageBitmap;
	void ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescriptorSize);

	void* RequestPage();

	void FreePage(void* address);
	void FreePages(void* address, uint64_t pageCount);
	void LockPage(void* address);
	void LockPages(void* address, uint64_t pageCount);

	uint64_t GetTotalRAM();
	uint64_t GetFreeRAM();
	uint64_t GetUsedRAM();
	uint64_t GetReservedRAM();
};

extern PageFrameAllocator GlobalAllocator;
