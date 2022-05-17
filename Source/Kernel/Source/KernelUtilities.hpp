#pragma once

#include <BootInfo.h>

#include "Memory/EFIMemory.hpp"
#include "Memory/Memory.hpp"
#include "Bitmap.hpp"
#include "Memory/Paging/PageFrameAllocator.hpp"
#include "Memory/Paging/Paging.hpp"
#include "Memory/Paging/PageTableManager.hpp"

// Used to get the size of the kernel, so we can reserve some memory for it
extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
	PageTableManager* pageTableManager;
};

KernelInfo InitialiseKernel(BootInfo* bootInfo);
