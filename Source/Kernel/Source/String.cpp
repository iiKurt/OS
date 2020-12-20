#include "String.hpp"

// Say hello to probably the most well commented file in the entire project.

#include <stddef.h>

char str[128]; // Buffer overflows?!
const char* to_string(int64_t number, unsigned int base, unsigned int desiredLength, bool prefix) {
	if (base < 2 || base > 36) { // no
		return NULL;
	}

	// Get a pointer to the end of the string.
	// If we want to prepend something, we can increment this pointer to offset any additonal characters.
	char* current = &str[0]; // Get a pointer to the start of the string.

	if (number < 0) 
	{
		number = -number; // Treat it as a normal numberber
		// Prepend '-'. This will set a character to the start of the string, then offset additional characters
		*current++ = '-';
	}

	// We don't know the length of the string - calculate it.
	if (desiredLength <= 0) {
		// Set length to one, as length calculation exits when stripping rightmost (remaining) digit
		// results in zero (but there is still one digit yet to be calculated - so we compensate for this:)
		int length = 1;
		int lengthTest = number;

		// lengthTest / base will only equal zero when the number is at 1 digit long, and we therefore know the length.
		// lengthTest is implicity converted to int each time, so decimals are stripped off.
		while (lengthTest / base != 0) { // While stripping the rightmost digit results in a non-zero digit.
			lengthTest /= base; // Strip rightmost digit.
			length++;
		}

		desiredLength = length;
	}

	// If longest possible string output would result in buffer overflow.
	// 4 because of: -, 0, [prefix symbol], \0
	if (desiredLength + 4 > 128) {
		return NULL;
	}

	if (prefix && base != 10) { // Base 10 doesn't need a prefix
		*current++ = '0';
		switch (base) {
			case 16: // Hexadecimal
				*current++ = 'x';
				break;
			case 8: // Octal
				*current++ = 'o';
				break;
			case 2: // Binary
				*current++ = 'b';
				break;
			default: // Some strange unknown base
				*current++ = '?';
				break;
		}
	}

	// Grab the address of where the prefixes end (subtracting 1 because there are no prefixes there).
	char* endPrefixes = current - 1;

	// We could be working in reverse, so set the current position to the end of the string.
	// Indexing starts from zero, so subtract one.
	current += (desiredLength - 1);
	
	if (number == 0) { // Handle zero explicity, otherwise nothing occurs
		*current-- = '0';
	}
	else {
		// Filling string from right to left.
		// TODO: This method could maybe be changed to one similar to the double to string function, filling left to right.
		while (number != 0 && endPrefixes < current)
		{
			int rem = number % base; // Get rightmost digit.
			// If number we want (rem) can't be represented in characters from 0-9, start counting from A.
			// As the range 0-Z is not contiguous within ASCII, if we want to use A-Z, subtract 10 first and begin offsetting.
			// Otherwise, just offset rem from the 0 character to find the desired symbol.
			// Could do rem + (rem > 9 ? 55 : '0');
			*current-- = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
			number = number/base; // 'Remove' rightmost digit by implicitly converting to an int, stripping the decimal.
		}
	}

	// Detect if there is a gap between the start/prefix and digits.
	while (endPrefixes != current) {
		*current-- = '0';
	}

	// Even though the str buffer is initally filled with \0's, we still need to add one at the end
	// due to the buffer being shared across several function calls.
	// Offsetting by endPrefixes because desiredLength doesn't account for prefixes.
	// current pointer is not at the end of the array due to it being decremented to fill the array in reverse.
	// Adding one because endPrefixes points to a filled character (not the place after the prefixes end).
	*(endPrefixes + desiredLength + 1) = '\0';
	return str;
}

const char* to_string(double number, unsigned int decimalPlaces) {
	// Cap the number of decimal places to 20.
	if (decimalPlaces > 20) {
		decimalPlaces = 20;
	}

	char* intPtr = (char*)to_string((int64_t)number);
	char* current = &str[0]; // Start from the start of the buffer.

	// Treat the number as a positive, negative sign has been added by previous call to to_string.
	if (number < 0) {
		number *= -1;
	}

	// Copy characters from intPtr (which only contains the whole number) to the current output.
	while (*intPtr != 0) {
		*current = *intPtr;
		intPtr++;
		current++;
	}

	// Add a period after the whole number.
	*current++ = '.';

	// Subtract whole number away to leave just the decimals.
	double newNumber = number - (int)number;

	for (uint8_t i = 0; i < decimalPlaces; i++) {
		// We will be grabbing the leftmost digits and appending it to the output buffer.
		newNumber *= 10; // Shift to the left by one place, resulting in one digit to the left of the decimal point.
		*current++ = (int)newNumber + '0'; // Get the leftmost number and convert it to a character.
		newNumber -= (int)newNumber; // Subtract whole number away to leave just the decimals.
	}

	*current = '\0'; // Null terminate the string, not incrementing because we aren't adding any characters after this.
	return str;
}
