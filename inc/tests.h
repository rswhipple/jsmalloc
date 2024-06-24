#ifndef TESTS_H_
#define TESTS_H_

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "main.h"


// =================== Chunk ===================
void chunk_base_create_test(void** state);
void chunk_split_test_success(void** state);
void chunk_split_test_failure(void** state);
void huge_chunk_allocate_test_success(void** state);
void huge_chunk_allocate_test_failure(void** state);
void chunk_free_test(void** state);
void huge_chunk_free_test(void** state);
void chunk_merge_test(void** state);
void try_merge_is_in_use_test(void** state);

// =================== Tiny Chunk ===================
void tiny_chunk_base_create_test(void** state);
void tiny_chunk_create_test(void** state);
void tiny_chunk_free_test(void** state);


// =================== Fast Cache ===================
void fast_cache_create_test(void** state);
void search_fast_cache_test(void** state);

// =================== Cache Table ===================
void cache_table_create_test(void** state);
void cache_table_set_and_get_test(void** state);

// =================== Unsorted Cache ===================
void search_unsorted_cache_test(void** state);
void search_unsorted_cache_null_test(void** state);
void search_unsorted_cache_large_test(void** state);

// =================== Sorted Cache ===================
void search_sorted_cache_test(void** state);
void search_sorted_cache_null_test(void** state);
// void search_sorted_cache_large_test(void** state);

#endif
