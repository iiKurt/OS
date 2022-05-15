#include "Painter.hpp"

Painter::Painter(Framebuffer* fb, PSF1_FONT* font) {
	this->FB = fb;
	this->Font = font;
}

// Voodoo magic ahead
void Painter::PrintCharacter(char chr, unsigned int xOff, unsigned int yOff, uint32_t primaryColor, uint32_t secondaryColor) {
	// There may be a better way to detect/implement transparency
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

// https://stackoverflow.com/a/41095728
// Big WTF
inline uint32_t rgba_interp(uint32_t src, uint32_t dst, uint32_t t) {
	const uint32_t s = 256 - t;
	return (
		(((((src >> 0)  & 0xff) * s +
		   ((dst >> 0)  & 0xff) * t) >> 8)) |
		(((((src >> 8)  & 0xff) * s +
		   ((dst >> 8)  & 0xff) * t)     )  & ~0xff) |
		(((((src >> 16) & 0xff) * s +
		   ((dst >> 16) & 0xff) * t) << 8)  & ~0xffff) |
		(((((src >> 24) & 0xff) * s +
		   ((dst >> 24) & 0xff) * t) << 16) & ~0xffffff)
	);
}

void Painter::DrawFilledRectangle(unsigned int xOff, unsigned int yOff, unsigned int w, unsigned int h, uint32_t color) {
	for (unsigned int x = 0; x < w; x++) {
		for (unsigned int y = 0; y < h; y++) {
			*(unsigned int*)((unsigned int*)FB->BaseAddress + (xOff + x) + ((yOff + y) * FB->PixelsPerScanLine)) = color;
		}
	}
}

// TODO: What if no FPU?
void Painter::DrawGradientRectangle(unsigned int xOff, unsigned int yOff, unsigned int w, unsigned int h,
                                    uint32_t sourceColor, uint32_t destinationColor, Orientation orientation) {
	uint32_t color = 0x00000000;

	// Compiler may optimise this: https://stackoverflow.com/q/1462710
	for (unsigned int x = 0; x < w; x++) {
		for (unsigned int y = 0; y < h; y++) {
			if (orientation == Orientation::Vertical) {
				// Fun math described here: https://stackoverflow.com/a/5732390
				// TODO: Could this be bit shifted?
				color = rgba_interp(sourceColor, destinationColor, ((double)256/(double)h) * y);
			}
			else if (orientation == Orientation::Horizontal) {
				color = rgba_interp(sourceColor, destinationColor, ((double)256/(double)w) * x);
			}
			
			*(unsigned int*)((unsigned int*)FB->BaseAddress + (xOff + x) + ((yOff + y) * FB->PixelsPerScanLine))
			 = color;
		}
	}
}

/* Alternate gradient drawing algorithm
void DrawGradientA(int x, int y, int w, int h, unsigned int color_start, unsigned int color_end, bool draw_horizontal)
{
	int off_a = (int)((color_end & 0xFF000000) >> 24) - (int)((color_start & 0xFF000000) >> 24);
	int off_r = (int)((color_end & 0x00FF0000) >> 16) - (int)((color_start & 0x00FF0000) >> 16);
	int off_g = (int)((color_end & 0x0000FF00) >> 8) - (int)((color_start & 0x0000FF00) >> 8);
	int off_b = (int)((color_end & 0x000000FF)) - (int)((color_start & 0x000000FF));

	int base_a = (int)((color_start & 0xFF000000) >> 24);
	int base_r = (int)((color_start & 0x00FF0000) >> 16);
	int base_g = (int)((color_start & 0x0000FF00) >> 8);
	int base_b = (int)((color_start & 0x000000FF));

	if(draw_horizontal)
	{
		int i, j;
		for(i = y; i < y+h; i++)
		{
			for(j = x; j < x+w; j++)
			{
				float fhStep = (j - x) / (float)w;
				uint32_t col_gradient = ((base_a + (int)(off_a*fhStep)) << 24) | ((base_r + (int)(off_r*fhStep)) << 16) | ((base_g + (int)(off_g*fhStep)) << 8) | (base_b + (int)(off_b*fhStep));

				DrawPixelA(j, i, col_gradient);
			}
		}
	}
	else
	{
		int i, j;
		for(i = y; i < y+h; i++)
		{
			float fhStep = (i - y) / (float)h;
			uint32_t col_gradient = ((base_a + (int)(off_a*fhStep)) << 24) | ((base_r + (int)(off_r*fhStep)) << 16) | ((base_g + (int)(off_g*fhStep)) << 8) | (base_b + (int)(off_b*fhStep));

			for(j = x; j < x+w; j++)
				DrawPixelA(j, i, col_gradient);
		}
	}
}
*/
