#include "Interrupts.hpp"
#include "../Panic.hpp"
#include "../IO.hpp"

#include "../Graphics/Painter.hpp"

void Halt() {
	for (;;) { __asm__("cli; hlt"); } // Halt the system, prevents a system reset
}

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame) {
	Panic("Page fault occured");
	Halt();
}

__attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame* frame) {
	Panic("Double fault occured");
	Halt();
}

__attribute__((interrupt)) void GPFault_Handler(struct interrupt_frame* frame) {
	Panic("General protection fault occured");
	Halt();
}

int testr = 0;
__attribute__((interrupt)) void KeyboardInt_Handler(struct interrupt_frame* frame) {
	GlobalPainter->PrintString("Key Pressed", 512, testr);
	testr += 16;
	uint8_t scancode = inb(0x60); // Port of PS/2 keyboard
	PIC_EndMaster();
}

void PIC_EndMaster() {
	outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave() {
	outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

// Remap PIC addresses so they don't collide with the exception addresses
void RemapPIC() {
	uint8_t a1, a2;
	a1 = inb(PIC1_DATA);
	io_wait();
	a2 = inb(PIC2_DATA);
	io_wait();

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

	outb(PIC1_DATA, 0x20);
	io_wait();
	outb(PIC2_DATA, 0x28);
	io_wait();

	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	// Restore bit masks
	outb(PIC1_DATA, a1);
	io_wait();
	outb(PIC2_DATA, a2);
}
