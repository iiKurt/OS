#include "Panic.hpp"
#include "Graphics/Console.hpp"

void Panic(const char* panicMessage) {
	Console panicConsole = Console(GlobalPainter);
	panicConsole.ForegroundColor = 0xFFFF0000;
	panicConsole.BackgroundColor = 0xFF000000;

	panicConsole.PrintLine("! Kernel Panic !");
	panicConsole.PrintLine();
	panicConsole.PrintLine(panicMessage);
}
