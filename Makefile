include ./Makefile.variables

SOURCEDIR	:= ./Source
BUILDDIR	:= ./Build
TOOLSDIR	:= ./Tools
BUNDLEDDIR	:= ./Bundled

# Pro tip: don't indent comments or have comments inline with a command.

# OVMF firmware
OVMF_URL	:= https://dl.bintray.com/no92/vineyard-binary/OVMF.fd
OVMF		:= $(TOOLSDIR)/OVMF.fd

# QEMU
HDD			:= $(BUILDDIR)/HDD
EMU			:= qemu-system-x86_64
# Was using -M accel=kvm:tcg
EMUFLAGS	:= -drive if=pflash,format=raw,file=$(OVMF) -drive format=raw,file=fat:rw:$(HDD) -m 256M -M accel=tcg -net none -serial stdio
IMG			:= $(BUILDDIR)/OS.img

MOUNTDIR	:= $(BUILDDIR)/Mount
EFIMOUNT	:= $(MOUNTDIR)/EFI
OSMOUNT		:= $(MOUNTDIR)/OS

# Run the emulator.
.PHONY: run
run: hdd $(OVMF) 
	@$(EMU) $(EMUFLAGS)

# Creates empty disk image when required.
$(IMG):
	@dd if=/dev/zero of=$(IMG) bs=512 count=93750 2>/dev/null

# Formats and copies files to disk image.
.PHONY: image
image: shared bootloader kernel $(IMG)
# Create paths for the EFI and OS partitions to be mounted to.
	@mkdir -p $(EFIMOUNT)
	@mkdir -p $(OSMOUNT)

# For macOS
ifeq ($(UNAME), Darwin)
# Create two partitions, one EFI (ef00) and the other a basic data (0700).
	sgdisk -o -n 1:0:+10M -t 1:ef00 -n 2:0:0 -t 2:0700 $(IMG) 2>/dev/null
# Bash script - can't have comments here without makefile breaking. Summary:
# Attach the image without mounting a filesystem, also grab the device node path.
# Format EFI partition as FAT16 (because smaller minimum size).
# Format OS partition as FAT32.
# Mount the filsystems.
	set -e;\
	device=$$(hdiutil attach -nomount -nobrowse "$(IMG)" | egrep '^/dev/' | sed 1q | awk '{print $$1}');\
	newfs_msdos -F 16 -v EFI "$${device}s1";\
	newfs_msdos -F 32 -v OS "$${device}s2";\
	mount -t msdos "$${device}"s1 $(EFIMOUNT);\
	mount -t msdos "$${device}"s2 $(OSMOUNT);
endif

# Copy the files
	@mkdir -p $(EFIMOUNT)/efi/boot
	@cp $(SOURCEDIR)/Bootloader/Build/bootx64.efi $(EFIMOUNT)/efi/boot/bootx64.efi
	@cp $(SOURCEDIR)/Kernel/Build/Kernel.elf $(EFIMOUNT)/Kernel.elf
	@cp $(BUNDLEDDIR)/zap-light16.psf $(EFIMOUNT)/zap-light16.psf

ifeq ($(UNAME), Darwin)
# Only need to detach one partition for the whole disk to be unmounted apparently.
	hdiutil detach $(EFIMOUNT)
#hdiutil detach $(OSMOUNT)
endif

# Downloads OVMF.
$(OVMF):
	@mkdir -p $(TOOLSDIR)
	wget $(OVMF_URL) -O $(OVMF) -qq

# Deletes ('cleans') the build directory, and runs the clean command on other components.
.PHONY: clean
clean:
	-@$(RM) -rf $(BUILDDIR)
# Clean bootloader.
	@$(MAKE) -C $(SOURCEDIR)/Bootloader clean
# Clean kernel.
	@$(MAKE) -C $(SOURCEDIR)/Kernel clean

####################
# Build components #
####################

.PHONY: shared
shared:
	@$(MAKE) -C $(SOURCEDIR)/Shared

# Make the bootloader.
.PHONY: bootloader
bootloader:
# Change directory first (-C) so makefile is in the context of its own diretory, instead of this one.
	@$(MAKE) -C $(SOURCEDIR)/Bootloader

.PHONY: kernel
kernel:
	@$(MAKE) -C $(SOURCEDIR)/Kernel

