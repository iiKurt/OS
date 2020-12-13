#include <BootInfo.h>

#include "Graphics/Painter.hpp"
#include "Graphics/Console.hpp"
#include "Memory/EFIMemory.hpp"
#include "Memory/Memory.hpp"
#include "String.hpp"
#include "Bitmap.hpp"
#include "Memory/PageFrameAllocator.hpp"

// Used to get the size of the kernel, so we can reserve some memory for it
extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start(BootInfo* bootInfo) {
	Painter p = Painter(bootInfo->fb, bootInfo->font);
	Console c = Console(&p);

	c.Clear();
	// Print some information about the system
	c.ForegroundColor = 0xFF00FF00;
	c.PrintLine("[System Info]");
	c.Print("Screen Width: ");
	c.PrintLine(to_string((uint64_t)bootInfo->fb->Width));
	c.Print("Screen Height: ");
	c.PrintLine(to_string((uint64_t)bootInfo->fb->Height));
	c.PrintLine("");
	c.ForegroundColor = 0xFFFFFFFF;

	PageFrameAllocator allocator;
	allocator.ReadEFIMemoryMap((EFI_MEMORY_DESCRIPTOR*)bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescriptorSize);

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

	allocator.LockPages(&_KernelStart, kernelPages);

	c.Print("Free RAM: ");
	c.Print(to_string(allocator.GetFreeRAM() / 1024));
	c.PrintLine(" KB");

	c.Print("Used RAM: ");
	c.Print(to_string(allocator.GetUsedRAM() / 1024));
	c.PrintLine(" KB");

	c.Print("Reserved RAM: ");
	c.Print(to_string(allocator.GetReservedRAM() / 1024));
	c.PrintLine(" KB");

	for (int i = 0; i < 20; i++) {
		void* address = allocator.RequestPage();
		c.PrintLine(to_hex_string((uint64_t)address));
	}

	// Number of map entries
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

	c.Print(to_string(GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize)));
	c.PrintLine(" bytes");

	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
