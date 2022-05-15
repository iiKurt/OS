include ./Makefile.variables

SOURCEDIR	:= ./Source
BUILDDIR	:= ./Build
TOOLSDIR	:= ./Tools
BUNDLEDDIR	:= ./Bundled

# Pro tip: don't indent comments or have comments inline with a command.

# OVMF firmware
OVMF_URL	:= https://github.com/iiKurt/OS/blob/a7f9ebf831c3a0603cc6385fb1bf70a3b10d0274/Tools/OVMF.fd?raw=true
OVMF		:= $(TOOLSDIR)/OVMF.fd

# QEMU
HDD			:= $(BUILDDIR)/HDD
IMG			:= $(BUILDDIR)/OS.img
EMU			:= qemu-system-x86_64
# Was using -M accel=kvm:tcg
EMUFLAGS	:= -drive if=pflash,format=raw,file=$(OVMF) -drive format=raw,file=fat:rw:$(HDD) -m 256M -M accel=tcg -net none -serial stdio

MOUNTDIR	:= $(BUILDDIR)/Mount
EFIMOUNT	:= $(MOUNTDIR)/EFI
OSMOUNT		:= $(MOUNTDIR)/OS

# TODO: Silence endless garbage that disk image related rules output.

################
# Common tasks #
################

.PHONY: default
default: image run

# Run the emulator.
.PHONY: run
run: $(IMG) $(OVMF)
	@$(EMU) $(EMUFLAGS)

# Deletes ('cleans') the build directory, and runs the clean command on other components.
.PHONY: clean
clean:
	-@$(RM) -rf $(BUILDDIR)
# Clean bootloader.
	@$(MAKE) -C $(SOURCEDIR)/Bootloader clean
# Clean kernel.
	@$(MAKE) -C $(SOURCEDIR)/Kernel clean

######################
# Construct OS image #
######################

.PHONY: hdd
hdd: shared bootloader kernel
	@mkdir -p $(HDD)/EFI/BOOT
	@cp $(SOURCEDIR)/Bootloader/Build/bootx64.efi $(HDD)/EFI/BOOT/bootx64.efi
	@cp $(SOURCEDIR)/Kernel/Build/Kernel.elf $(HDD)/Kernel.elf
	@cp $(BUNDLEDDIR)/zap-light16.psf $(HDD)/zap-light16.psf

# Creates empty disk image when required.
# TODO: Confusing rules: $(IMG) and image.
$(IMG):
	@mkdir -p $(BUILDDIR)
# Create the empty disk image.
	@dd if=/dev/zero of=$(IMG) bs=512 count=93750 1>/dev/null
# Create two partitions, one EFI (ef00) and the other a basic data (0700).
	@sgdisk -o -n 1:0:+10M -t 1:ef00 -n 2:0:0 -t 2:0700 $(IMG) 1>/dev/null

# Formats and copies files to a raw disk image.
# Note that VirtualBox doesn't like raw disk images. Use 'VBoxManage convertdd' to make a bootable .vdi.
# https://stackoverflow.com/a/57432808
# https://unix.stackexchange.com/a/617506
.PHONY: image
image: hdd $(IMG)
# Create paths for the EFI and OS partitions to be mounted to.
	@mkdir -p $(EFIMOUNT)
	@mkdir -p $(OSMOUNT)

# For macOS.
ifeq ($(UNAME), Darwin)
# Bash script - can't have comments here without makefile breaking. Summary:
# Attach the image without mounting a filesystem, also grab the device node path.
# Format EFI partition as FAT16 (because smaller minimum size).
# Format OS partition as FAT32.
# Mount the filesystems.
	@set -e;\
	device=$$(hdiutil attach -nomount -nobrowse "$(IMG)" | egrep '^/dev/' | sed 1q | awk '{print $$1}') 1>/dev/null;\
	newfs_msdos -F 16 -v EFI "$${device}s1" 1>/dev/null;\
	newfs_msdos -F 32 -v OS "$${device}s2" 1>/dev/null;\
	mount -t msdos "$${device}"s1 $(EFIMOUNT) 1>/dev/null;\
	mount -t msdos "$${device}"s2 $(OSMOUNT) 1>/dev/null;
endif

# Copy the files from the HDD to the disk image.
	@cp -r $(HDD)/. $(EFIMOUNT)

ifeq ($(UNAME), Darwin)
# Only need to detach one partition for the whole disk to be unmounted apparently.
	@hdiutil detach $(EFIMOUNT)
#hdiutil detach $(OSMOUNT)
endif

# Downloads OVMF.
$(OVMF):
	@mkdir -p $(TOOLSDIR)
	wget $(OVMF_URL) -O $(OVMF) -qq

####################
# Build components #
####################

.PHONY: shared
shared:
	@$(MAKE) -C $(SOURCEDIR)/Shared

# Make the bootloader.
.PHONY: bootloader
bootloader:
# Change directory first (-C) so makefile is in the context of its own directory, instead of this one.
	@$(MAKE) -C $(SOURCEDIR)/Bootloader

.PHONY: kernel
kernel:
	@$(MAKE) -C $(SOURCEDIR)/Kernel

