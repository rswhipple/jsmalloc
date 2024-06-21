#include "../../inc/tests.h"

void search_unsorted_cache_test(void** state) {
  UNUSED(state);
  size_t size = 100;
  t_chunk* unsorted_ck = g_pagemap->span_head->page_head->next->top_chunk;
  // printf("unsorted_ck->size: %zu\n", unsorted_ck->size);
  size_t new_unsorted_size = unsorted_ck->size - size;
  // printf("new_unsorted_size: %zu\n", new_unsorted_size);
  unsorted_ck->fd = g_pagemap->frontend_cache->unsorted_cache;
  g_pagemap->frontend_cache->unsorted_cache = unsorted_ck;
  
  
  void* result = search_unsorted_cache(size);

  assert_ptr_not_equal(result, NULL);
  assert_int_equal(new_unsorted_size, g_pagemap->frontend_cache->unsorted_cache->size);
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
  t_chunk* unsorted_ck = g_pagemap->span_head->page_head->next->top_chunk;
  size_t new_unsorted_size = unsorted_ck->size - size;
  unsorted_ck->fd = g_pagemap->frontend_cache->unsorted_cache;
  g_pagemap->frontend_cache->unsorted_cache = unsorted_ck;
  
  
  void* result = search_unsorted_cache(size);

  assert_ptr_not_equal(result, NULL);
  assert_int_equal(new_unsorted_size, g_pagemap->frontend_cache->unsorted_cache->size);


}

