#include "BasicRenderer.hpp"

extern "C" void _start(Framebuffer* fb, PSF1_FONT* font) {
	BasicRenderer br = BasicRenderer(fb, font);
	br.CursorPosition = {0, 0};

	for (int i = 0; i < 50; i++) {
		br.PrintStr("It's a kernel!");
	}

	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
