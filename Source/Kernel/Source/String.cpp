#include "String.hpp"

char stringOutput[128];
// TODO: perhaps somehow combine both uint64 and int64 to_string logic
const char* to_string(uint64_t value) {
	uint8_t size;
	uint64_t sizeTest = value;

	// For each time sizeTest can be divided by 10, increment size
	// Could use modulus and a for loop to do this
	while (sizeTest / 10 > 0) {
		sizeTest /= 10;
		size++;
	}

	uint8_t index = 0;

	while (value / 10 > 0) {
		uint8_t remainder = value % 10;
		value /= 10;
		stringOutput[size - index] = remainder + '0';
		index++;
	}

	uint8_t remainder = value % 10;
	stringOutput[size - index] = remainder + '0';
	stringOutput[size + 1] = 0; // Null-terminate the string
	return stringOutput;
}

const char* to_string(int64_t value) {
	uint8_t isNegative = 0;

	if (value < 0) {
		isNegative = 1;
		value *= -1;
		// For all intents and purposes it can be positive
		// Prepend a - to the returned characters
		stringOutput[0] = '-';
	}

	uint8_t size;
	uint64_t sizeTest = value;

	while (sizeTest / 10 > 0) {
		sizeTest /= 10;
		size++;
	}

	uint8_t index = 0;

	while (value / 10 > 0) {
		uint8_t remainder = value % 10;
		value /= 10;
		stringOutput[isNegative + size - index] = remainder + '0';
		index++;
	}

	uint8_t remainder = value % 10;
	stringOutput[isNegative + size - index] = remainder + '0';
	stringOutput[isNegative + size + 1] = 0; // Null-terminate the string
	return stringOutput;
}

const char* to_string(double value, uint8_t decimalPlaces) {
	if (decimalPlaces > 20) {
		decimalPlaces = 20;
	}

	char* intPtr = (char*)to_string((int64_t)value);
	char* doublePtr = stringOutput;

	if (value < 0) {
		value *= -1;
	}

	while (*intPtr != 0) {
		*doublePtr = *intPtr;
		intPtr++;
		doublePtr++;
	}

	*doublePtr = '.';
	doublePtr++;

	double newValue = value - (int)value;

	for (uint8_t i = 0; i < decimalPlaces; i++) {
		newValue *= 10;
		*doublePtr = (int)newValue + '0';
		newValue -= (int)newValue;
		doublePtr++;
	}

	*doublePtr = 0;
	return stringOutput;
}

const char* generic_to_hex_string(uint8_t* valPtr, uint8_t size) {

	size -= 1;
	uint8_t* ptr;
	uint8_t temp;

	for (uint8_t i = 0; i < size; i++) {
		ptr = (valPtr + i);
		temp = ((*ptr & 0xF0) >> 4);
		stringOutput[2 + size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
		temp = ((*ptr & 0x0F));
		stringOutput[2 + size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
	}

	// These have to be at the bottom of this function
	stringOutput[0] = '0';
	stringOutput[1] = 'x';

	stringOutput[2 + size + 1] = 0;
	return stringOutput;
}

const char* to_hex_string(uint64_t value) {
	return generic_to_hex_string((uint8_t*)&value, 16); // Will only show last 16 characters
}

const char* to_hex_string(uint32_t value) {
	return generic_to_hex_string((uint8_t*)&value, 8);
}

const char* to_hex_string(uint16_t value) {
	return generic_to_hex_string((uint8_t*)&value, 4);
}

const char* to_hex_string(uint8_t value) {
	return generic_to_hex_string((uint8_t*)&value, 2);
}
