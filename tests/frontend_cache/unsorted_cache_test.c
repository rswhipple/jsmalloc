#include "../../inc/tests.h"

void search_unsorted_cache_test(void** state) {
  UNUSED(state);
  size_t size = 100;
  size_t top_chunk_size = CHUNK_SIZE(g_pagemap->top_chunk);
  free_chunk(g_pagemap->top_chunk, top_chunk_size);
  void* result = search_unsorted_cache(size);

  assert_ptr_not_equal(result, NULL);
  assert_int_equal(CHUNK_SIZE(g_pagemap->top_chunk), 0);

  t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
  assert_ptr_equal(unsorted_chunk, NULL);
}

void search_unsorted_cache_null_test(void** state) {
  UNUSED(state);
  size_t size = 100;
  void* result = search_unsorted_cache(size);
  assert_null(result);
}

void search_unsorted_cache_large_test(void** state) {
  UNUSED(state);
  size_t size = 1000;
  size_t top_chunk_size = CHUNK_SIZE(g_pagemap->top_chunk);
  free_chunk(g_pagemap->top_chunk, top_chunk_size);
  void* result = search_unsorted_cache(size);
  assert_ptr_not_equal(result, NULL);

  t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
  assert_ptr_equal(unsorted_chunk, NULL);

}

void search_unsorted_cache_small_test(void** state) {
  UNUSED(state);
  size_t size = 16;
  size_t top_chunk_size = CHUNK_SIZE(g_pagemap->top_chunk);
  free_chunk(g_pagemap->top_chunk, top_chunk_size);
  void* result = search_unsorted_cache(size);
  assert_ptr_not_equal(result, NULL);
  t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
  assert_ptr_equal(unsorted_chunk, NULL);
}