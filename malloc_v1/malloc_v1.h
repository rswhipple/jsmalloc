#ifndef MALLOC_V1_H  
#define MALLOC_V1_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>

#define MEMORY_SHIFT(start, data_size)((char *)start + data_size)
#define UNUSED(x) (void)(x)

// =================== V1 Heap ===================

typedef struct s_heap t_heap;
typedef struct s_block t_block;

struct s_heap {
  struct s_heap* prev;
  struct s_heap* next;
  size_t total_size;
  size_t free_size;
  int block_count;
};

struct s_block {
  struct s_block* prev;
  struct s_block* next;
  size_t data_size;
  bool freed;
  void* object;
};

extern t_heap* global_heap;

#define SMALL_HEAP_ALLOCATION_SIZE 20
#define LARGE_HEAP_ALLOCATION_SIZE 20
#define SMALL_PAGE_MAX_CHUNK_SIZE 512
#define LARGE_PAGE_MAX_CHUNK_SIZE (LARGE_HEAP_ALLOCATION_SIZE * PAGE_SIZE / 12)

void* create_block(t_heap* heap, size_t size);

#define HEAP_SHIFT(start)((char *)start + sizeof(t_heap))
#define BLOCK_SHIFT(start)((char *)start + sizeof(t_block))

#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_BLOCK_SIZE (SMALL_PAGE_ALLOCATION_SIZE / 128)
#define SMALL_PAGE_ALLOCATION_SIZE 20

#define MAX_BLOCKS 20 

void create_heap(t_heap** heap, size_t size);

void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);
void* my_calloc(size_t nitems, size_t size);
void* search_heap(size_t size, int category);
int get_page_type(size_t size);

#endif