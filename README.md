# OS
Operating System

## Building
### Required Utilities

| Utility                         | Category      | Notes                                                                  |
|---------------------------------|---------------|------------------------------------------------------------------------|
| wget                            | Initial Setup |                                                                        |
| Make                            | Build         |                                                                        |
| Clang, LLD                      | Compilation   | Component of LLVM, used to compile EFI Bootloader and cross compiler.  |
| QEMU                            | Execution     | Can use another emulator or run on real hardware.                      |
| dd                              | Distrubution  |                                                                        |
| sgdisk                          | Distrubition  | Component of the gptfdisk package, used to lay out disk image.         |
| [A FAT32 filesystem formatter]  | Distrubution  | Used to format the disk image. Currently hard coded as newfs_msdos.    |
| [A disk image mounter]          | Distrubution  | Used to copy files to the disk image. Currently hard coded as hdiutil. |
| my-little-pony-expansion-pack-3 | ???           |                                                                        |

### Instructions
1. Run ./Setup.sh to download and install a x86_64-elf cross compiler to ./Tools/Cross
    - Some prerequisites for GCC will be downloaded and are not required to be installed on your system.
    - You may delete any redundant directories from ./Tools
2. Run `make`
    - The OS will be built
    - A raw disk image will be created at ./Build/OS.img
    - OVMF will be downloaded to ./Tools/OVMF.fd
    - Emulation will begin in QEMU
