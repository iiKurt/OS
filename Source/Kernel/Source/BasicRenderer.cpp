#include "BasicRenderer.hpp"

BasicRenderer::BasicRenderer(Framebuffer* fb, PSF1_FONT* font) {
	this->FB = fb;
	this->Font = font;
	this->Color = 0xffffffff;
}

// Voodoo magic ahead
void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff) {
	unsigned int* pixPtr = (unsigned int*)FB->BaseAddress;
	// Multiply character we want to print by the height of the character
	// Character we want can be found by adding offset onto the glyphBuffer starting point
	char* fontPtr = (char*)Font->glyphBuffer + (chr * Font->psf1_Header->charsize); // Indexing

	for (unsigned long y = yOff; y < yOff + 16; y++) { // 16 bits high
		for (unsigned long x = xOff; x < xOff + 8; x++) { // 8 bits long
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) { // Select the bit that we need
				*(unsigned int*)(pixPtr + x + (y * FB->PixelsPerScanLine)) = Color;
			}
			else { // It is not a colored pixel
				// Alpha channel is first two nibbles (1 byte)
				*(unsigned int*)(pixPtr + x + (y * FB->PixelsPerScanLine)) = 0x000000FF;
			}
		}
		fontPtr++;
	}
}

void BasicRenderer::PrintStr(const char* str) {
	char* chr = (char*)str;

	while (*chr != 0) {
		if (CursorPosition.X + 8 > FB->Width) { // If incrementing the cursor position will result in a character being drawn off screen
			CursorPosition.X = 0;
			CursorPosition.Y += 16;
		}
		PutChar(*chr, CursorPosition.X, CursorPosition.Y);

		CursorPosition.X += 8; // Move 8 pixels to the right
		chr++;
	}
}
