#pragma once

#include "Painter.hpp"

// TODO: scroll mode, overwrite from top or scroll existing lines? (will have to fill and rewrite screen)

class Console {
private:
	Painter* painter;
	Point cursorPosition;

public:
	unsigned int ForegroundColor;
	unsigned int BackgroundColor;

	Console(Painter* painter);

	void Print(const char* str);
	void PrintLine(const char* str);
	void Clear();
};
