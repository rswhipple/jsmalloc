#include "../inc/tests.h"
#include <stdlib.h>


static int setup(void** state) {
    system_settings();

    create_pagemap(&g_pagemap);

    *state = g_pagemap;
    return 0;
}

static int teardown(void** state) {
    UNUSED(state);
    // TODO: uncomment this when destry working fully?
    // destroy_pagemap((t_pagemap*)state);

    return 0;
}


int main(void) {
    const struct CMUnitTest tests[] = {
        // =================== Chunk ===================
        cmocka_unit_test_setup_teardown(create_top_chunk_test, setup, teardown),
        cmocka_unit_test_setup_teardown(split_chunk_test_success, setup, teardown),
        cmocka_unit_test_setup_teardown(split_chunk_test_failure, setup, teardown),
        cmocka_unit_test_setup_teardown(allocate_huge_chunk_test_success, setup, teardown),
        cmocka_unit_test_setup_teardown(allocate_huge_chunk_test_failure, setup, teardown),
        cmocka_unit_test_setup_teardown(free_chunk_test, setup, teardown),
        cmocka_unit_test_setup_teardown(free_huge_chunk_test, setup, teardown),

        // =================== Tiny Chunk ===================
        cmocka_unit_test_setup_teardown(create_top_tiny_chunk_test, setup, teardown),
        cmocka_unit_test_setup_teardown(create_tiny_chunk_test, setup, teardown),
        cmocka_unit_test_setup_teardown(free_tiny_chunk_test, setup, teardown),

        // =================== Fast Cache ===================
        cmocka_unit_test_setup_teardown(create_fast_cache_test, setup, teardown),
        cmocka_unit_test_setup_teardown(search_fast_cache_test, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
