#include "../../inc/tests.h"

void cache_table_create_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_cache* cache = pagemap->frontend_cache;
  t_cache_table* table = cache_table_create(cache);
  assert_ptr_not_equal(table, NULL);
  assert_int_equal(table->capacity, NUM_BINS);
}

void cache_table_set_test(void** state) {
    UNUSED(state);
//   t_pagemap* pagemap = (t_pagemap*)*state;
//   t_cache* cache = pagemap->frontend_cache;

}

void cache_table_get_test(void** state) {
    UNUSED(state);
//   t_pagemap* pagemap = (t_pagemap*)*state;
//   t_cache* cache = pagemap->frontend_cache;

}