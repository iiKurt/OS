# defines the compiler and linker paths and arguments
include ../../Makefile.variables

BUILDDIR		:= Build
SOURCEDIR		:= Source

#OBJECTS			:= \

# prepend BUILDDIR to each object file
OBJECTS			:= $(addprefix $(BUILDDIR)/, $(OBJECTS))

DEPENDENCIES	:= $(patsubst %.o,%.d,$(OBJECTS))

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPENDENCIES)

clean:
	-@rm -rf $(BUILDDIR)
