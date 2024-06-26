CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -O0
LDFLAGS := -lcmocka -L/usr/local/lib
SRCDIR = ../src
TESTDIR = .
INCDIR = ../include
BUILDDIR = build
TEST_BUILDDIR = $(BUILDDIR)/tests

# Create necessary directories
$(shell mkdir -p $(TEST_BUILDDIR))

# Collect all source files excluding ../src/main.c
SOURCES = $(shell find $(SRCDIR) -name '*.c')
TEST_SOURCES = $(shell find $(TESTDIR) -name '*_test.c')

# Generate object file paths
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
TEST_OBJECTS = $(patsubst $(TESTDIR)/%.c,$(TEST_BUILDDIR)/%.o,$(TEST_SOURCES))

# Include directories
INCLUDES = -I$(INCDIR) -I/opt/homebrew/include

# Target for the test executable
TEST_TARGET = $(TEST_BUILDDIR)/run

all: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -D TESTING -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -D TESTING $(INCLUDES) -c -o $@ $<

$(TEST_BUILDDIR)/%.o: $(TESTDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -D TESTING $(INCLUDES) -c -o $@ $<

test: $(TEST_TARGET)
	./build/tests/run

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TEST_TARGET)
