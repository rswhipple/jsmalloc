// my_malloc.h
#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <unistd.h>

void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);
void* my_calloc(size_t nitems, size_t size);
void* search_heap(size_t size, int category);
int get_page_type(size_t size);

#endif // MY_MALLOC_H
