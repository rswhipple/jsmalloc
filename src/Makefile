CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
LDFLAGS = -fsanitize=address

# Using CURDIR to set the directories
SRCDIR = $(CURDIR)
INCDIR = $(CURDIR)/../inc
BUILDDIR = $(CURDIR)/../build
LIBDIR = $(BUILDDIR)/lib
LIBRARY = $(LIBDIR)/libmymalloc.a

# Recursively gather all source files
SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
INCLUDES = -I$(INCDIR)

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	@mkdir -p $(LIBDIR)
	ar rcs $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(LIBRARY)
