#pragma once

#include <stdint.h>

#include "Point.hpp"
#include <Framebuffer.h>
#include <PSF.h>

enum class Orientation { Horizontal, Vertical };

class Painter {
public:
	Framebuffer* FB;
	PSF1_FONT* Font;
	unsigned int Color;
	unsigned int SecondaryColor;

	Painter(Framebuffer* fb, PSF1_FONT* font);

	void PrintCharacter(char chr, unsigned int xOff, unsigned int yOff, uint32_t primaryColor, uint32_t secondaryColor);
	void PrintString(const char* str, unsigned int x, unsigned int y);
	void DrawFilledRectangle(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t color);
	void DrawGradientRectangle(unsigned int xOff, unsigned int yOff, unsigned int w, unsigned int h,
                               uint32_t sourceColor, uint32_t destinationColor, Orientation orientation = Orientation::Vertical);
};
