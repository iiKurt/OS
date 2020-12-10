#pragma once

#include "Math.hpp"
#include <Framebuffer.h>
#include <PSF.h>

class BasicRenderer {
public:
	Point CursorPosition;
	Framebuffer* FB;
	PSF1_FONT* Font;
	unsigned int Color;

	BasicRenderer(Framebuffer* fb, PSF1_FONT* font);

	void PutChar(char chr, unsigned int xOff, unsigned int yOff);
	void PrintStr(const char* str);
};
