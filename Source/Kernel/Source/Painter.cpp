#include "Painter.hpp"

Painter::Painter(Framebuffer* fb, PSF1_FONT* font) {
	this->FB = fb;
	this->Font = font;
}

// Voodoo magic ahead
void Painter::PrintCharacter(char chr, unsigned int xOff, unsigned int yOff, unsigned int primaryColor, unsigned int secondaryColor) {
	// here may be a better way to detect/implement transparency
	char isTransparent = 0;

	if ((secondaryColor >> 24) == 0x00) { // Alpha channel is zero
		isTransparent = 1;
	}

	unsigned int* pixPtr = (unsigned int*)FB->BaseAddress;
	// Multiply character we want to print by the height of the character
	// Character we want can be found by adding offset onto the glyphBuffer starting point
	char* fontPtr = (char*)Font->glyphBuffer + (chr * Font->psf1_Header->charsize); // Indexing

	for (unsigned long y = yOff; y < yOff + 16; y++) { // 16 bits high
		for (unsigned long x = xOff; x < xOff + 8; x++) { // 8 bits long
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) { // Select the bit that we need
				*(unsigned int*)(pixPtr + x + (y * FB->PixelsPerScanLine)) = primaryColor;
			}
			else if (!isTransparent) { // It is not a colored pixel
				// Alpha channel is first two nibbles (1 byte)
				*(unsigned int*)(pixPtr + x + (y * FB->PixelsPerScanLine)) = secondaryColor;
			}
		}
		fontPtr++;
	}
}

void Painter::PrintString(const char* str, unsigned int x, unsigned int y) {
	char* chr = (char*)str;

	while (*chr != 0) {
		// TODO: would be nice to draw partial characters off screen
		if (x + 8 > FB->Width) { // If incrementing the cursor position will result in a character being drawn off screen
			return; // don't
		}

		PrintCharacter(*chr, x, y, 0xFFFFFFFF, 0x00000000);

		x += 8; // Move 8 pixels to the right
		chr++;
	}
}
