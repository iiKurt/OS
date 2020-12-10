#include "BasicRenderer.hpp"
#include "String.hpp"

extern "C" void _start(Framebuffer* fb, PSF1_FONT* font) {
	BasicRenderer br = BasicRenderer(fb, font);
	br.CursorPosition = {0, 0};

	for (int i = 0; i < 50; i++) {
		br.PrintStr("It's a kernel!");
	}

	br.CursorPosition = {0, br.CursorPosition.Y + 32};
	br.PrintStr(to_string((uint64_t)1003892));

	br.CursorPosition = {0, br.CursorPosition.Y + 16};
	br.PrintStr(to_string((int64_t)-1003892));

	br.CursorPosition = {0, br.CursorPosition.Y + 16};
	br.PrintStr(to_string((double)37.1337));

	br.CursorPosition = {0, br.CursorPosition.Y + 16};
	br.PrintStr(to_string((double)-37.1337));

	br.CursorPosition = {0, br.CursorPosition.Y + 16};
	br.PrintStr(to_hstring((uint64_t)0x1234ABCD));

	br.CursorPosition = {0, br.CursorPosition.Y + 16};
	br.PrintStr(to_hstring((uint32_t)0x1234ABCDF0F0));

	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
