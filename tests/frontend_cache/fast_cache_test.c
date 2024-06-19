#include "../../inc/tests.h"

void create_fast_cache_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_cache* cache = pagemap->frontend_cache;
  t_tiny_chunk** fast_cache = create_fast_cache(cache);
  for (size_t i = 0; i < cache->fcache_size; ++i) {
    assert_null(fast_cache[i]);
  }
}

void search_fast_cache_test(void** state) {
  UNUSED(state);
  size_t size = 24;
  void* result = search_fast_cache(size);
  assert_ptr_not_equal(result, NULL);
  void* result_two = search_fast_cache(size);
  assert_ptr_not_equal(result_two, NULL);
  assert_ptr_not_equal(result, result_two);
}