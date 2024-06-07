#ifndef CHUNKS_H
#define CHUNKS_H

#include "types.h"

void write_boundary_tag(t_chunk* chunk);
t_chunk* create_top_chunk(t_page* page);
t_chunk* split_chunk(t_chunk* chunk, size_t size);

void print_tiny_chunk(t_tiny_chunk* tiny);
t_tiny_chunk* create_top_tiny_chunk(t_fpage* page);
t_tiny_chunk* create_tiny_chunk(t_fpage* fpage);

t_chunk* allocate_huge_chunk(size_t size);

#endif