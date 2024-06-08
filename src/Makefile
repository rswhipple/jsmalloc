CC = gcc
CFLAGS = -Wall -Wextra -Werror 

# Using CURDIR to set the directories
SRCDIR = $(CURDIR)
INCDIR = $(CURDIR)/../inc
BUILDDIR = $(CURDIR)/../build
TARGET = $(BUILDDIR)/malloc

# Recursively gather all source files
SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
INCLUDES = -I$(INCDIR)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)
