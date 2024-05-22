
#ifndef HEAP_H
#define HEAP_H

#include "my_malloc.h"

t_chunk *create_chunk(t_page *page);
t_tiny_chunk *create_tiny_chunk(t_page *page, size_t size);
t_chunk *split_chunk(t_chunk *chunk, size_t size);

#endif