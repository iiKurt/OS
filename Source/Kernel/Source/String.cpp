#include "String.hpp"

// Say hello to probably to most well commented file in the entire project.

#include <stddef.h>

char str[128]; // Buffer overflows?!
const char* to_string(int64_t num, int base, unsigned int desiredLength, bool prefix) {
	if (base < 2 || base > 36) { // no
		return NULL;
	}

	// We don't know the length of the string - calculate it.
	if (desiredLength <= 0) {
		// Set length to one, as length calculation exits when stripping rightmost (remaining) digit
		// results in zero (but there is still one digit yet to be calculated - so we compensate for this:)
		int length = 1;
		int lengthTest = num;

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

	// Get a pointer to the end of the string.
	// If we want to prepend something, we can increment this pointer to offset any additonal characters.
	char* current = &str[0]; // Get a pointer to the start of the string.

	if (num < 0) 
	{
		num = -num; // Treat it as a normal number
		// Prepend '-'. This will set a character to the start of the string, then offset additional characters
		*current++ = '-';
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

	// Grab the address of where the prefixes are (subtracting 1 because that has not been filled yet).
	char* endPrefixes = current - 1;

	// We could be working in reverse, so set the current position to the end of the string.
	// Indexing starts from zero, so subtract one.
	current += (desiredLength - 1);
	
	if (num == 0) { // Handle zero explicity, otherwise nothing occurs
		*current-- = '0';
	}
	else {
		// Filling string from right to left.
		while (num != 0 && endPrefixes < current)
		{
			int rem = num % base; // Get rightmost digit.
			// If number we want (rem) can't be represented in characters from 0-9, start counting from A.
			// As the range 0-Z is not contiguous within ASCII, if we want to use A-Z, subtract 10 first and begin offsetting.
			// Otherwise, just offset rem from the 0 character to find the desired symbol.
			// Could do rem + (rem > 9 ? 55 : '0');
			*current-- = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
			num = num/base; // 'Remove' rightmost digit by implicitly converting to an int, stripping the decimal.
		}
	}

	// Detect if there is a gap between the start/prefix and digits
	while (endPrefixes != current) {
		*current-- = '0';
	}

	// Even though the str buffer is initally filled with \0's, we still need to add one at the end
	// due to the buffer being shared across several function calls.
	// Offsetting by endPrefixes because desiredLength doesn't account for prefixes.
	// current pointer is not at the end of the array due to it being decremented to fill the array in reverse.
	*(endPrefixes + desiredLength + 1) = '\0';
	return str;
}
