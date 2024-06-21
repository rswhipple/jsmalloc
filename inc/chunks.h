#ifndef CHUNKS_H
#define CHUNKS_H

#include "types.h"

void chunk_write_boundary_tag(t_chunk* chunk);
t_chunk* chunk_top_create(t_page* page);
t_chunk* chunk_split(t_chunk* chunk, size_t size);
t_chunk* chunk_merge(t_chunk* value_1, t_chunk* value_2);
t_chunk* try_merge(t_chunk* value);
void chunk_free(void* ptr, size_t size);

void tiny_chunk_print(t_tiny_chunk* tiny);
t_tiny_chunk* tiny_chunk_top_create(t_fpage* page);
t_tiny_chunk* tiny_chunk_create(t_fpage* fpage);
void tiny_chunk_free(void* ptr, size_t size);

t_chunk* huge_chunk_allocate(size_t size);
void huge_chunk_free(void* ptr, size_t size);


#endif