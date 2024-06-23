#include "../../inc/tests.h"

void search_sorted_cache_test(void** state) {
  UNUSED(state);
  t_pagemap* pagemap = (t_pagemap*)*state;

  t_chunk* top_chunk = pagemap->top_chunk;
  size_t size = round_up_to_next(72);
  t_chunk* chunk1 = chunk_split(top_chunk, size);
  top_chunk = pagemap->top_chunk;
  SET_FREE(chunk1);
  cache_table_set(chunk1);
  void* result = search_sorted_cache(72, small);
  assert_ptr_not_equal(result, NULL);
  t_chunk* result_ck = DATA_TO_CHUNK(result);
  assert_int_equal(result_ck->size, 88);

  size = round_up_to_next(100);
  t_chunk* chunk2 = chunk_split(top_chunk, size);
  SET_FREE(chunk2);
  result = search_sorted_cache(100, small);
  assert_ptr_not_equal(result, NULL);
  result_ck = DATA_TO_CHUNK(result);
}

void search_sorted_cache_null_test(void** state) {
  UNUSED(state);
  size_t size = 100;
  void* result = search_unsorted_cache(size);
  assert_null(result);
}