# defines the compiler and linker paths and arguments
include ../../Makefile.variables

# object file generation path
BUILDDIR		:= Build
# modules path
SOURCEDIR		:= Source

# Extend the makefile.variables
#CFLAGS			+=
LDFLAGS			+= -T Kernel.ld

# output binary path
KERNEL			:= $(BUILDDIR)/Kernel.elf

OBJECTS			:= \
	Kernel.o

# prepend BUILDDIR to each object file
OBJECTS			:= $(addprefix $(BUILDDIR)/, $(OBJECTS))

# there should be a dep file for every object file
DEPENDENCIES	:= $(patsubst %.o,%.d,$(OBJECTS))

# link the object files into the ELF file under Build/kernel.elf, if objects are newer than the ELF file
$(KERNEL): $(OBJECTS)
	@$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

# rules for module files
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# include the dependencies if they exist
-include $(DEPENDENCIES)

clean:
	-@rm -rf $(BUILDDIR)
