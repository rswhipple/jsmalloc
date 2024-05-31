#include "../inc/tests.h"

void null_test_success(void** state) {
  (void)state;
}

void int_test_success(void** state) {
  int* answer = (int*)*state;
  assert_int_equal(*answer, 42);
}