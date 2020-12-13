#include <BootInfo.h>

#include "Graphics/Painter.hpp"
#include "Graphics/Console.hpp"
#include "Memory/EFIMemory.hpp"
#include "String.hpp"

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

	// Print EFI memory sections
	// Number of map entries
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

	// Iterate through each map entry
	for (unsigned int i = 0; i < mMapEntries; i++) {
		// Get the descriptor at an offset to the memory map
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescriptorSize));
		
		// Print the corrosponding type string to the console
		c.Print(EFI_MEMORY_TYPE_STRINGS[desc->type]);
		c.ForegroundColor = 0xFFFF00FF;
		c.Print(" ");
		// pageCount is the number of 4KiB pages
		// Calculate how many KiBs by multiplying by 4
		c.Print(to_string(desc->pageCount << 2));
		// Do ((pageCount * 4) * 1024)/1000 to calculate kB
		// Or ((pageCount * 4096)/1000)
		c.PrintLine(" KiB");
		c.ForegroundColor = 0xFFFFFFFF;
	}

	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
