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
  assert_int_not_equal(success, 1);
  void* result = cache_table_get(chunk1->size);
  assert_ptr_equal(result, chunk1);

  success = cache_table_set(chunk2);
  assert_int_not_equal(success, 1);
  result = cache_table_get(chunk2->size);
  assert_ptr_equal(result, chunk2);

  success = cache_table_set(chunk3);
  assert_int_not_equal(success, 1);
  result = cache_table_get(chunk3->size);
  assert_ptr_equal(result, chunk3);
}

