#include "IO.hpp"

void outb(uint16_t port, uint8_t value) {
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
	uint8_t returnValue;
	asm volatile ("inb %1, %0"
		: "=a"(returnValue)
		: "Nd"(port));
	return returnValue;
}

void io_wait() {
	asm volatile ("outb %%al, $0x80" : : "a"(0)); // Send a value to unused port
}
