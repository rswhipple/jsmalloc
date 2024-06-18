#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "main.h"

#ifndef TESTS_H_
#define TESTS_H_


// =================== Chunk ===================
void create_top_chunk_test(void** state);
void split_chunk_test_success(void** state);
void split_chunk_test_failure(void** state);
void allocate_huge_chunk_test_success(void** state);
void allocate_huge_chunk_test_failure(void** state);
void free_chunk_test(void** state);
void free_huge_chunk_test(void** state);

// =================== Tiny Chunk ===================
void create_top_tiny_chunk_test(void** state);
void create_tiny_chunk_test(void** state);
void free_tiny_chunk_test(void** state);


// =================== Fast Cache ===================
void create_fast_cache_test(void** state);
void search_fast_cache_test(void** state);

#endif
