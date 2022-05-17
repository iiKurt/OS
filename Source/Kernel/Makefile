# defines the compiler and linker paths and arguments
include ../../Makefile.variables

ASMC			:= nasm
ASMFLAGS		:= 

# object file generation path
BUILDDIR		:= Build
# modules path
SOURCEDIR		:= Source

# Extend the makefile.variables
#CXXFLAGS		+= -O3
LDFLAGS			+= -T Kernel.ld

# output binary path
KERNEL			:= $(BUILDDIR)/Kernel.elf

OBJECTS			:= \
	Kernel.o \
	KernelUtilities.o \
	Panic.o \
	IO.o \
	Graphics/Painter.o \
	Graphics/Console.o \
	GDT/LoadGDT.o \
	GDT/GDT.o \
	Interrupts/IDT.o \
	Interrupts/Interrupts.o \
	Memory/EFIMemory.o \
	Memory/Memory.o \
	Memory/Paging/PageFrameAllocator.o \
	Memory/Paging/PageMapIndexer.o \
	Memory/Paging/PageTableManager.o \
	Memory/Paging/Paging.o \
	Bitmap.o \
	String.o

# prepend BUILDDIR to each object file
OBJECTS			:= $(addprefix $(BUILDDIR)/, $(OBJECTS))

# there should be a dep file for every object file
DEPENDENCIES	:= $(patsubst %.o,%.d,$(OBJECTS))

# link the object files into the ELF file under Build/kernel.elf, if objects are newer than the ELF file
$(KERNEL): $(OBJECTS)
	@$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)

$(BUILDDIR)/Interrupts/Interrupts.o: $(SOURCEDIR)/Interrupts/Interrupts.cpp
	@mkdir -p $(dir $@)
	@$(CXX) -I$(SHARED) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $< -o $@

# rules for module files
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.asm
	@mkdir -p $(dir $@)
	@$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@

# include the dependencies if they exist
-include $(DEPENDENCIES)

.PHONY: clean
clean:
	-@rm -rf $(BUILDDIR)
