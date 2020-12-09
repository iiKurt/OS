#include <Framebuffer.h>
#include <PSF.h>

typedef struct {
	unsigned int X;
	unsigned int Y;
} Point;

// Voodoo magic ahead
void putChar(Framebuffer* fb, PSF1_FONT* font, unsigned int color, char chr, unsigned int xOff, unsigned int yOff) {
	unsigned int* pixPtr = (unsigned int*)fb->BaseAddress;
	// Multiply character we want to print by the height of the character
	// Character we want can be found by adding offset onto the glyphBuffer starting point
	char* fontPtr = font->glyphBuffer + (chr * font->psf1_Header->charsize); // Indexing

	for (unsigned long y = yOff; y < yOff + 16; y++) { // 16 bits high
		for (unsigned long x = xOff; x < xOff + 8; x++) { // 8 bits long
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) { // Select the bit that we need
				*(unsigned int*)(pixPtr + x + (y * fb->PixelsPerScanLine)) = color;
			}
			else { // It is not a colored pixel
				// Alpha channel is first two nibbles (1 byte)
				*(unsigned int*)(pixPtr + x + (y * fb->PixelsPerScanLine)) = 0x000000FF;
			}
		}
		fontPtr++;
	}
}

Point CursorPosition;
void print(Framebuffer* fb, PSF1_FONT* font, unsigned int color, char* str) {
	char* chr = str;

	while (*chr != 0) {
		if (CursorPosition.X + 8 > fb->Width) { // If incrementing the cursor position will result in a character being drawn off screen
			CursorPosition.X = 0;
			CursorPosition.Y += 16;
		}
		putChar(fb, font, color, *chr, CursorPosition.X, CursorPosition.Y);

		CursorPosition.X += 8; // Move 8 pixels to the right
		chr++;
	}
}

void _start(Framebuffer* fb, PSF1_FONT* font) {
	CursorPosition.X = 0;
	CursorPosition.Y = 0;

	for (int i = 0; i < 50; i++) {
		print(fb, font, 0xffffffff, "It's a kernel!");
	}

	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
