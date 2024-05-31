CC = gcc
CFLAGS = -Wall -Wextra -Werror  -fsanitize=address -g3

# Using CURDIR to set the SRCDIR
SRCDIR = $(CURDIR)
INCDIR = $(CURDIR)/../inc
BUILDDIR = $(CURDIR)/../build
TARGET = $(BUILDDIR)/malloc

# Gather all source files recursively
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
INCLUDES = -I$(INCDIR)

all: $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
		$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
		rm -f $(OBJECTS) $(TARGET)
