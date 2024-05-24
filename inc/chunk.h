
#ifndef CHUNK_H
#define CHUNK_H

#include "types.h"

t_chunk* create_top_chunk(t_page* page);
t_tiny_chunk* create_top_tiny_chunk(t_fpage* page);
t_chunk* split_chunk(t_chunk* chunk, size_t size);
t_tiny_chunk* split_tiny_chunk(t_page* page, t_tiny_chunk* prev_chunk, size_t size);

#endif