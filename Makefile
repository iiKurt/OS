# Based off: https://wiki.osdev.org/User:No92/UEFI_Bare_Bones

include ./Makefile.variables

SOURCEDIR	:= ./Source
BUILDDIR	:= ./Build

# pro tip: don't indent comments or have comments inline with a command

# QEMU
HDD			:= $(BUILDDIR)/HDD
EMU			:= qemu-system-x86_64
EMUFLAGS	:= -drive if=pflash,format=raw,file=$(BUILDDIR)/OVMF.fd -drive format=raw,file=fat:rw:$(HDD) -M accel=kvm:tcg -net none -serial stdio

# OVMF firmware
OVMF_URL	:= https://dl.bintray.com/no92/vineyard-binary/OVMF.fd
OVMF		:= $(BUILDDIR)/OVMF.fd

# executes the emulator
.PHONY: execute
execute: hdd $(OVMF) 
	$(EMU) $(EMUFLAGS)

# construct the drive used by the emulator
.PHONY: hdd
hdd: bootloader kernel
	mkdir -p $(HDD)/efi/boot
	cp $(SOURCEDIR)/Bootloader/Build/bootx64.efi $(HDD)/efi/boot/bootx64.efi
	cp $(SOURCEDIR)/Kernel/Build/kernel.elf $(HDD)/kernel.elf

# downloads OVMF
$(OVMF):
	mkdir -p $(BUILDDIR)
	wget $(OVMF_URL) -O $(OVMF) -qq

# deletes ('cleans') the build directory, and runs the clean command on other components
.PHONY: clean
clean:
	$(RM) -r $(BUILDDIR)
# clean bootloader
	$(MAKE) -C $(SOURCEDIR)/Bootloader clean
# clean kernel
	$(MAKE) -C $(SOURCEDIR)/Kernel clean

####################
# Build components #
####################

# make the bootloader
.PHONY: bootloader
bootloader:
# change directory first (-C) so makefile is in the context of its own diretory, instead of this one
	$(MAKE) -C $(SOURCEDIR)/Bootloader

.PHONY: kernel
kernel:
	$(MAKE) -C $(SOURCEDIR)/Kernel

