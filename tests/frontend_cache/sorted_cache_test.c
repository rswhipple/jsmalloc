#include "../../inc/tests.h"

void search_sorted_cache_test(void** state) {
  UNUSED(state);
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
  
  void* result = search_sorted_cache(72, small);
  assert_ptr_not_equal(result, NULL);
  assert_int_equal(result, (void*)(chunk1 + pointer_size));

  result = search_sorted_cache(100, small);
  assert_ptr_not_equal(result, NULL);
  assert_int_equal(result, (void*)(chunk2 + pointer_size));
//   result = search_sorted_cache(200, small);
//   assert_ptr_not_equal(result, NULL);
//   assert_int_equal(result, (void*)(chunk3 + pointer_size));

}

void search_sorted_cache_null_test(void** state) {
  UNUSED(state);
  size_t size = 100;
  void* result = search_unsorted_cache(size);
  assert_null(result);
}
