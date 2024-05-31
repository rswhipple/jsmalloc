#include "../inc/tests.h"
#include <stdlib.h>

static int setup(void** state) {
    int* answer = (int*)malloc(sizeof(int));
    if (answer == NULL) {
        return -1;
    }
    *answer = 42;

    *state = answer;

    return 0;
}

static int teardown(void** state) {
    free(*state);

    return 0;
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test_setup_teardown(int_test_success, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
