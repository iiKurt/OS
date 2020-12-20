# OS
Operating System

## Building
### Required Utilities
- Clang (Used to compile EFI bootloader and cross compiler)
- Make
- dd (For creating a blank disk image)
- sgdisk (gptfdisk) (For laying out said disk image)
- Some kind of filesystem formatter and disk image mounter (Currently hard coded to use OS X's hdiutil and newfs)
- QEMU (Or another emulator)
- my-little-pony-expansion-pack-3

### Instructions
1. Run ./Setup.sh to download and install a x86_64-elf cross compiler to ./Tools/Cross
    - Some prerequisites for GCC will be downloaded and are not required to be installed on your system.
    - You may delete any redundant directories from ./Tools
2. Run `make`
    - The OS will be built
    - OVMF will be downloaded to ./Tools/OVMF.fd
    - Emulation will begin in QEMU
