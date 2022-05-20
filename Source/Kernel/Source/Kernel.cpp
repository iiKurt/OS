#include "Kernel.hpp"

#include <BootInfo.h>

#include "KernelUtilities.hpp"
#include "String.hpp"

Console* KernelConsole;
extern "C" void _start(BootInfo* bootInfo) {
	KernelInfo kernelInfo = InitialiseKernel(bootInfo);
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	Console c = Console(GlobalPainter);
	KernelConsole = &c;

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

	c.Print("Total RAM: ");
	c.Print(to_string(GlobalAllocator.GetTotalRAM() / 1024));
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

	while (true);
	//for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
