#pragma once

#include <stddef.h>
#include <stdint.h>

class Bitmap {
public:
	// TODO: perhaps move Size and Buffer to constructor and make them private
	size_t Size;
	uint8_t* Buffer;
	bool operator[](uint64_t index) const;
	// https://stackoverflow.com/a/46820522
	// https://stackoverflow.com/a/11066591
	bool Set(uint64_t index, bool value);
};
