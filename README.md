# Welcome to My Malloc

## Task

Create your own implementation of the malloc families functions in order to allocate memory:

- my_malloc
- my_free
- my_calloc
- my_realloc

## Description

How did we solve it? See:

[Design doc](./JSMalloc.md)


## Installation

In your project directory:

1. `git clone https://git.us.qwasar.io/my_malloc_168649_if1t4-/my_malloc.git`
2. `cd my_malloc && make test run_tests` to confirm all tests are passing
3. `cd .. && touch main.c` and add the following:

   ```c
   #include "./my_malloc/inc/main.h" and `make`:

   int main() {
       void* ptr = my_malloc(100);
       printf("ptr: %p\n", ptr);

       return 0;
   }
   ```

4. `touch Makefile` and add the following then `make`:

   ```Makefile
   CC = gcc
   CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3 -I./my_malloc/include -I./include
   LDFLAGS := -L./my_malloc/build/lib -lmymalloc
   SRCDIR = ./my_malloc/src
   BUILDDIR = ./my_malloc/build
   LIBDIR = $(BUILDDIR)/lib
   LIBRARY = $(LIBDIR)/libmymalloc.a

   # Collect all source files recursively from my_malloc/src
   SOURCES := $(shell find $(SRCDIR) -name '*.c')
   OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/src/%.o, $(SOURCES))

   # Ensure build directories exist
   $(shell mkdir -p $(LIBDIR))
   $(shell mkdir -p $(BUILDDIR)/src)

   all: $(LIBRARY) my_project

   $(LIBRARY): $(OBJECTS)
     $(AR) rcs $@ $^

   $(BUILDDIR)/src/%.o: $(SRCDIR)/%.c
     @mkdir -p $(dir $@)
     $(CC) $(CFLAGS) -c -o $@ $<

   my_project: $(BUILDDIR)/main.o $(LIBRARY)
     $(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

   $(BUILDDIR)/main.o: main.c
     @mkdir -p $(dir $@)
     $(CC) $(CFLAGS) -c -o $@ $<

   clean:
     rm -rf $(BUILDDIR)

   .PHONY: all clean
   ```

## Usage

run `./my_project` to test malloc functions directly in your main

### The Core Team

Rebecca Whipple Silverstein and Mathius Johnson

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>