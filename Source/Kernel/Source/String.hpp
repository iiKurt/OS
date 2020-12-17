#pragma once

#include <stdint.h>

const char* to_string(int64_t number, unsigned int base = 10, unsigned int desiredLength = 0, bool prefix = true);
const char* to_string(double number, unsigned int decimalPlaces);
