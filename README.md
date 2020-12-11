# OS
Operating System

## Building
### Required Utilities
- Clang (Used to compile EFI bootloader and cross compiler)
- Make
- QEMU (Or another emulator)

### Instructions
1. Run ./Setup.sh to download and install a x86_64-elf cross compiler to ./Tools/Cross
		- Some prerequistes for GCC will be downloaded and are not required to be installed on your system.
		- You may delete any redundant directories from ./Tools
2. Run `make`
		- The OS will be built
		- OVMF will be downloaded to ./Tools/OVMF.fd
		- Emulation will begin in QEMU
