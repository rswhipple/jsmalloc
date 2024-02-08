# Compiler
CC = gcc

# Compiler flags
CFLAGS=-Wall -fsanitize=address -g3

# Source file
SRC = my_malloc.c

# Executable name
TARGET = my_malloc

# Default target, which will be executed when you just type "make" without any arguments
all: $(TARGET)

# Rule to compile the C source file into an executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up the generated files
clean:
	rm -f $(TARGET)

fclean: clean
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean