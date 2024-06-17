#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "main.h"

#ifndef TESTS_H_
#define TESTS_H_

void create_top_chunk_test(void** state);
void split_chunk_test_success(void** state);
void split_chunk_test_failure(void** state);
void allocate_huge_chunk_test_success(void** state);
void allocate_huge_chunk_test_failure(void** state);
void free_chunk_test(void** state);
void free_huge_chunk_test(void** state);

#endif
