# Based off: https://wiki.osdev.org/User:No92/UEFI_Bare_Bones

include ./Makefile.variables

SOURCEDIR	:= ./Source
BUILDDIR	:= ./Build
TOOLSDIR	:= ./Tools
BUNDLEDDIR	:= ./Bundled

# pro tip: don't indent comments or have comments inline with a command

# OVMF firmware
OVMF_URL	:= https://dl.bintray.com/no92/vineyard-binary/OVMF.fd
OVMF		:= $(TOOLSDIR)/OVMF.fd

# QEMU
HDD			:= $(BUILDDIR)/HDD
EMU			:= qemu-system-x86_64
# was using -M accel=kvm:tcg
EMUFLAGS	:= -drive if=pflash,format=raw,file=$(OVMF) -drive format=raw,file=fat:rw:$(HDD) -M accel=tcg -net none -serial stdio

# executes the emulator
.PHONY: execute
execute: hdd $(OVMF) 
	@$(EMU) $(EMUFLAGS)

# construct the drive used by the emulator
.PHONY: hdd
hdd: shared bootloader kernel
	@mkdir -p $(HDD)/efi/boot
	@cp $(SOURCEDIR)/Bootloader/Build/bootx64.efi $(HDD)/efi/boot/bootx64.efi
	@cp $(SOURCEDIR)/Kernel/Build/Kernel.elf $(HDD)/Kernel.elf
	@cp $(BUNDLEDDIR)/zap-light16.psf $(HDD)/zap-light16.psf

# downloads OVMF
$(OVMF):
	@mkdir -p $(TOOLSDIR)
	wget $(OVMF_URL) -O $(OVMF) -qq

# deletes ('cleans') the build directory, and runs the clean command on other components
.PHONY: clean
clean:
	-@$(RM) -rf $(BUILDDIR)
# clean bootloader
	@$(MAKE) -C $(SOURCEDIR)/Bootloader clean
# clean kernel
	@$(MAKE) -C $(SOURCEDIR)/Kernel clean

####################
# Build components #
####################

.PHONY: shared
shared:
	@$(MAKE) -C $(SOURCEDIR)/Shared

# make the bootloader
.PHONY: bootloader
bootloader:
# change directory first (-C) so makefile is in the context of its own diretory, instead of this one
	@$(MAKE) -C $(SOURCEDIR)/Bootloader

.PHONY: kernel
kernel:
	@$(MAKE) -C $(SOURCEDIR)/Kernel

