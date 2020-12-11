#include <BootInfo.h>

#include "Painter.hpp"
#include "Console.hpp"
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

	for (int i = 0; i < 40; i++) {
		c.Print("Number: ");
		c.PrintLine(to_string((int64_t)i));
	}

	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
