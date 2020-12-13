#include <BootInfo.h>

#include "Graphics/Painter.hpp"
#include "Graphics/Console.hpp"
#include "Memory/EFIMemory.hpp"
#include "Memory/Memory.hpp"
#include "String.hpp"
#include "Bitmap.hpp"

uint8_t testBuffer[20];

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

	Bitmap testBitmap;
	testBitmap.Buffer = &testBuffer[0];
	testBitmap.Set(0, false);
	testBitmap.Set(1, true);
	testBitmap.Set(2, false);
	testBitmap.Set(3, false);
	testBitmap.Set(4, true);
	testBitmap.Set(16, true);

	for (int i = 0; i < 20; i++) {
		c.PrintLine(testBitmap[i] ? "true" : "false");
	}

	// Number of map entries
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

	c.Print(to_string(GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize)));
	c.PrintLine(" bytes");


	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
