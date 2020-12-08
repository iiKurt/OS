void _start() {
	for (;;) { __asm__("cli; hlt"); } // Halt the system
	return;
}
