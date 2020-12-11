#pragma once

#include "Point.hpp"
#include <Framebuffer.h>
#include <PSF.h>

class Painter {
public:
	Framebuffer* FB;
	PSF1_FONT* Font;
	unsigned int Color;
	unsigned int SecondaryColor;

	Painter(Framebuffer* fb, PSF1_FONT* font);

	void PrintCharacter(char chr, unsigned int xOff, unsigned int yOff, unsigned int primaryColor, unsigned int secondaryColor);
	void PrintString(const char* str, unsigned int x, unsigned int y);
};
