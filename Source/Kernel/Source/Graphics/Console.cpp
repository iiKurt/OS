#include "Console.hpp"

Console::Console(Painter* painter) {
	this->painter = painter;
	this->cursorPosition = {0, 0};
	this->ForegroundColor = 0xFFFFFFFF;
	this->BackgroundColor = 0x00000000;
}

void Console::Print(const char* str) {
	char* chr = (char*)str;

	while (*chr != 0) {
		if (cursorPosition.X + 8 > painter->FB->Width) { // If incrementing the cursor position will result in a character being drawn off screen
			cursorPosition.X = 0;
			cursorPosition.Y += 16;
		}
		painter->PrintCharacter(*chr, cursorPosition.X, cursorPosition.Y, ForegroundColor, BackgroundColor);

		cursorPosition.X += 8; // Move 8 pixels to the right
		chr++;
	}
}

void Console::PrintLine(const char* str) {
	Print(str);
	cursorPosition.X = 0;
	cursorPosition.Y += 16;
}

void Console::Reset() {
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
}

void Console::Clear() {
	Reset();
	painter->DrawFilledRectangle(0, 0, painter->FB->Width, painter->FB->Height, BackgroundColor);
}

void Console::Clear(uint32_t sourceColor, uint32_t destinationColor) {
	Reset();
	painter->DrawGradientRectangle(0, 0, painter->FB->Width, painter->FB->Height, sourceColor, destinationColor);
}
