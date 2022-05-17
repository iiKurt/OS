#include "Interrupts.hpp"
#include "../Graphics/Painter.hpp"

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame) {
	GlobalPainter->PrintString("Page fault occured", 0, 0);
	for (;;) { __asm__("cli; hlt"); } // Halt the system, prevents a system reset
}
