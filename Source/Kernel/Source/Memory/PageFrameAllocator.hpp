#pragma once

#include <stdint.h>

#include "EFIMemory.hpp"
#include "../Bitmap.hpp"
#include "Memory.hpp"

class PageFrameAllocator {
private:
	void InitBitmap(size_t bitMapSize, void* bufferAddress);
public:
	Bitmap PageBitmap;
	void ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescriptorSize);
};
