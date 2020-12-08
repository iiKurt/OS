# Based off: https://stackoverflow.com/a/48267562, and help from https://stackoverflow.com/a/40621556

# This file is kinda somewhat special because Micro$oft uses their own PE format, so I am forced to compile it
# using Clang (or build a second cross compiler targeting PE). Avert your eyes, children. (poor practices probably ahead)

# defines the compiler and linker paths and arguments
include ../../Makefile.variables
# ^ not even sure if I should be including this with the amount of customisations I'm making...

CC				:= /usr/local/opt/llvm/bin/clang
LD				:= /usr/local/opt/llvm/bin/lld-link
TARGET			:= x86_64-pc-win32-coff

CFLAGS			+= -target $(TARGET) 
# yes, the := is intentional
LDFLAGS			:= -subsystem:efi_application -dll
TARGET			:= x86_64-pc-win32-coff

# object file generation path
BUILDDIR		:= Build
# modules path
SOURCEDIR		:= Source
# output binary path
BOOTLOADER		:= $(BUILDDIR)/bootx64.efi

# _could_ automatically populate this but eh
OBJECTS			:= \
	EFI/EFI.o \
	Bootloader.o \
	Services/LoadFile.o

# prepend BUILDDIR to each object file
OBJECTS			:= $(addprefix $(BUILDDIR)/, $(OBJECTS))

# there should be a dep file for every object file
DEPENDENCIES	:= $(patsubst %.o,%.d,$(OBJECTS))

# link the object files into the EFI file under Build/Binary/bootx64.efi, if objects are newer than the EFI file
$(BOOTLOADER): $(OBJECTS)
# using $(OBJECTS) to link all objects instead of $<, which only does first object file
	$(LD) $(LDFLAGS) -entry:efi_main $(OBJECTS) -out:$@

# rules for module files
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# include the dependencies if they exist
# pro tip: this command's gotta be at the bottom or otherwise bad things happen...
# ...try to remove an object file from the list and make twice, it won't throw the error a second time!
-include $(DEPENDENCIES)

clean:
	rm -rf $(BUILDDIR)
