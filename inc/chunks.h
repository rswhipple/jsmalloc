
#ifndef CHUNK_H
#define CHUNK_H

#include "types.h"

t_chunk* create_top_chunk(t_page* page);
t_chunk* split_chunk(t_chunk* chunk, size_t size);

void print_tiny_chunk(t_tiny_chunk* tiny);
t_tiny_chunk* create_top_tiny_chunk(t_fpage* page);
t_tiny_chunk* create_tiny_chunk(t_fpage* fpage);

#endif