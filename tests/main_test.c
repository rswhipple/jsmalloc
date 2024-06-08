#include "../inc/tests.h"
#include <stdlib.h>

static int setup(void** state) {
    system_settings();

    t_pagemap* pagemap = NULL;

    create_pagemap(&pagemap);

    *state = pagemap;
    return 0;
}

static int teardown(void** state) {
    UNUSED(state);
    // TODO: uncomment this when destry working fully?
    destroy_pagemap((t_pagemap*)state);

    return 0;
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test_setup_teardown(chunk_test_success, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
