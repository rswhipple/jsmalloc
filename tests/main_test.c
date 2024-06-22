#include "../inc/tests.h"
#include <stdlib.h>


static int setup(void** state) {
    system_settings();

    pagemap_create(&g_pagemap);
    /* pagemap_create() integrated function path:
    frontend_cache_create() -> fast_cache_create()
    frontend_cache_create() -> cache_table_create()
    span_base_create() -> pages_create() -> page_base_create() -> chunk_top_create()
    pages_create() -> page_create() -> chunk_top_create()
    span_base_create() -> fpages_create() - fpage_base_create() -> tiny_chunk_top_create()
    fpages_create() - fpage_create() -> tiny_chunk_top_create()
    */

    *state = g_pagemap;
    return 0;
}

static int teardown(void** state) {
    pagemap_destroy((t_pagemap*)state);

    return 0;
}


int main(void) {
    const struct CMUnitTest tests[] = {
        // =================== Chunk ===================
        cmocka_unit_test_setup_teardown(chunk_top_create_test, setup, teardown),
        cmocka_unit_test_setup_teardown(chunk_split_test_success, setup, teardown),
        cmocka_unit_test_setup_teardown(huge_chunk_allocate_test_success, setup, teardown),
        cmocka_unit_test_setup_teardown(huge_chunk_allocate_test_failure, setup, teardown),
        cmocka_unit_test_setup_teardown(chunk_free_test, setup, teardown),
        cmocka_unit_test_setup_teardown(huge_chunk_free_test, setup, teardown),
        cmocka_unit_test_setup_teardown(chunk_merge_test, setup, teardown),
        cmocka_unit_test_setup_teardown(try_merge_is_in_use_test, setup, teardown),

        // =================== Tiny Chunk ===================
        cmocka_unit_test_setup_teardown(tiny_chunk_top_create_test, setup, teardown),
        cmocka_unit_test_setup_teardown(tiny_chunk_create_test, setup, teardown),
        cmocka_unit_test_setup_teardown(tiny_chunk_free_test, setup, teardown),

        // =================== Fast Cache ===================
        cmocka_unit_test_setup_teardown(fast_cache_create_test, setup, teardown),
        cmocka_unit_test_setup_teardown(search_fast_cache_test, setup, teardown),

        // =================== Unsorted Cache ===================
        cmocka_unit_test_setup_teardown(search_unsorted_cache_test, setup, teardown),
        cmocka_unit_test_setup_teardown(search_unsorted_cache_null_test, setup, teardown),
        cmocka_unit_test_setup_teardown(search_unsorted_cache_large_test, setup, teardown),

        // =================== Sorted Cache ===================
        // cmocka_unit_test_setup_teardown(search_sorted_cache_test, setup, teardown),
        // cmocka_unit_test_setup_teardown(search_sorted_cache_null_test, setup, teardown),
        // cmocka_unit_test_setup_teardown(search_sorted_cache_large_test, setup, teardown),

        // =================== Cache Table ===================
        cmocka_unit_test_setup_teardown(cache_table_create_test, setup, teardown),
        cmocka_unit_test_setup_teardown(cache_table_set_and_get_test, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
