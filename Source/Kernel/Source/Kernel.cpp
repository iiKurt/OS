#include <BootInfo.h>

#include "Graphics/Painter.hpp"
#include "Graphics/Console.hpp"
#include "Memory/EFIMemory.hpp"
#include "Memory/Memory.hpp"
#include "String.hpp"
#include "Bitmap.hpp"
#include "Memory/PageFrameAllocator.hpp"

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

	// Number of map entries
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

	c.Print(to_string(GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize)));
	c.PrintLine(" bytes");


	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
