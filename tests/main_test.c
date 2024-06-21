#include "../inc/tests.h"
#include <stdlib.h>


static int setup(void** state) {
    system_settings();

    create_pagemap(&g_pagemap);

    *state = g_pagemap;
    return 0;
}

static int teardown(void** state) {
    destroy_pagemap((t_pagemap*)state);

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
        cmocka_unit_test_setup_teardown(merge_chunks_test, setup, teardown),
        // cmocka_unit_test_setup_teardown(try_merge_is_in_use_test, setup, teardown),

        // =================== Tiny Chunk ===================
        cmocka_unit_test_setup_teardown(create_top_tiny_chunk_test, setup, teardown),
        cmocka_unit_test_setup_teardown(create_tiny_chunk_test, setup, teardown),
        cmocka_unit_test_setup_teardown(free_tiny_chunk_test, setup, teardown),

        // =================== Fast Cache ===================
        cmocka_unit_test_setup_teardown(create_fast_cache_test, setup, teardown),
        cmocka_unit_test_setup_teardown(search_fast_cache_test, setup, teardown),

        // =================== Unsorted Cache ===================
        cmocka_unit_test_setup_teardown(search_unsorted_cache_test, setup, teardown),
        cmocka_unit_test_setup_teardown(search_unsorted_cache_null_test, setup, teardown),
        cmocka_unit_test_setup_teardown(search_unsorted_cache_large_test, setup, teardown),

        // =================== Cache Table ===================
        cmocka_unit_test_setup_teardown(cache_table_create_test, setup, teardown),
        // cmocka_unit_test_setup_teardown(cache_table_set_test, setup, teardown),
        // cmocka_unit_test_setup_teardown(cache_table_get_test, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
