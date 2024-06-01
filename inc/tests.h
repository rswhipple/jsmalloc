#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "main.h"

#ifndef TESTS_H_
#define TESTS_H_

void null_test_success(void** state);
void int_test_success(void** state);
void chunk_test_success(void** state);

#endif
