#include "../../inc/tests.h"

void cache_table_create_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_cache* cache = pagemap->frontend_cache;
  t_cache_table* table = cache_table_create(cache);
  assert_ptr_not_equal(table, NULL);
  assert_int_equal(table->capacity, NUM_BINS);
}

void cache_table_set_and_get_test(void** state) {
  // create chunks
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_cache* table = pagemap->frontend_cache->cache_table;
  t_chunk* top_chunk = pagemap->top_chunk;
  t_chunk* chunk1 = chunk_split(top_chunk, 72);
  top_chunk = pagemap->top_chunk;
  t_chunk* chunk2 = chunk_split(top_chunk, 100);
  top_chunk = pagemap->top_chunk;
  t_chunk* chunk3 = chunk_split(top_chunk, 212);
  SET_FREE(chunk1);
  SET_FREE(chunk2);
  SET_FREE(chunk3);

  // add chunks to cache_table
  int success;
  success = cache_table_set(chunk1);


}

