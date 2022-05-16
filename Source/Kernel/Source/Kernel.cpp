#include <BootInfo.h>

#include "Graphics/Painter.hpp"
#include "Graphics/Console.hpp"
#include "Memory/EFIMemory.hpp"
#include "Memory/Memory.hpp"
#include "String.hpp"
#include "Bitmap.hpp"
#include "Memory/Paging/PageFrameAllocator.hpp"
#include "Memory/Paging/Paging.hpp"
#include "Memory/Paging/PageTableManager.hpp"

// Used to get the size of the kernel, so we can reserve some memory for it
extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start(BootInfo* bootInfo) {
	Painter p = Painter(bootInfo->fb, bootInfo->font);
	Console c = Console(&p);

	c.Clear(0xFF000000, 0xFF0000FF);
	// Print some information about the system
	c.ForegroundColor = 0xFF00FF00;
	c.PrintLine("[System Info]");
	c.Print("Screen Width: ");
	c.PrintLine(to_string(bootInfo->fb->Width));
	c.Print("Screen Height: ");
	c.PrintLine(to_string(bootInfo->fb->Height));
	c.PrintLine("");
	c.ForegroundColor = 0xFFFFFFFF;

	// Number of map entries
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

	GlobalAllocator = PageFrameAllocator();
	GlobalAllocator.ReadEFIMemoryMap((EFI_MEMORY_DESCRIPTOR*)bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescriptorSize);

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

	GlobalAllocator.LockPages(&_KernelStart, kernelPages);

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memset(PML4, 0, 0x1000);

	PageTableManager pageTableManager = PageTableManager(PML4);
	for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize); t += 0x1000) {
		pageTableManager.MapMemory((void*)t, (void*)t);
	}

	uint64_t fbBase = (uint64_t)bootInfo->fb->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->fb->BufferSize + 0x1000;
	for (uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000) {
		pageTableManager.MapMemory((void*)t, (void*)t);
	}

	asm ("mov %0, %%cr3" : : "r" (PML4));

	c.PrintLine("Printing from the new Page Map!!");
	c.PrintLine("");

	c.Print("Total Memory: ");
	c.Print(to_string(GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize) / 1024));
	c.PrintLine(" KB");

	c.Print("Free RAM: ");
	c.Print(to_string(GlobalAllocator.GetFreeRAM() / 1024));
	c.PrintLine(" KB");

	c.Print("Used RAM: ");
	c.Print(to_string(GlobalAllocator.GetUsedRAM() / 1024));
	c.PrintLine(" KB");

	c.Print("Reserved RAM: ");
	c.Print(to_string(GlobalAllocator.GetReservedRAM() / 1024));
	c.PrintLine(" KB");

	c.PrintLine("");

	for (int i = 0; i < 20; i++) {
		void* address = GlobalAllocator.RequestPage();
		c.PrintLine(to_string((int64_t)address, 16, 16));
	}

	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
