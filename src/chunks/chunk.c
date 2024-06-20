#include "../../inc/main.h" 


void write_boundary_tag(t_chunk* chunk) {
    size_t* boundary_tag = (size_t*)((char*)chunk + CHUNK_SIZE(chunk) - sizeof(size_t));
    *boundary_tag = chunk->size;
}

t_chunk* create_top_chunk(t_page* page) {
    t_chunk* chunk = (t_chunk*)PAGE_SHIFT(page);
    chunk->size = page->memory;
    SET_FREE(chunk);
    write_boundary_tag(chunk);
    chunk->fd = NULL;
    chunk->bk = NULL;
    page->top_chunk = chunk;

    return chunk;
}

// input parameters are t_chunk *chunk and size_t chunk_size
t_chunk* split_chunk(t_chunk* chunk, size_t size) {
    if (CHUNK_SIZE(chunk) <= size) {
        fprintf(stderr, "Invalid split size: %zu (chunk size: %zu)\n", size, chunk->size);
        return NULL;
    }

    // Placeholder variables
    t_chunk* temp = chunk->fd;
    size_t initial_chunk_size = chunk->size;

    // Update the original chunk's size, free status, next pointer & boundary_tag
    t_chunk* first_chunk = chunk;
    first_chunk->size = size;
    write_boundary_tag(chunk);
    SET_IN_USE(first_chunk);    // set in_use after writing boundary tag 
    first_chunk->fd = NULL;
    first_chunk->bk = NULL;

    // Create the second chunk immediately after the first chunk
    t_chunk* second_chunk = (t_chunk*)MEMORY_SHIFT(CHUNK_SHIFT(chunk), size);
    second_chunk->size = initial_chunk_size - size;
    SET_FREE(second_chunk);
    second_chunk->bk = chunk;
    second_chunk->fd = temp;
    write_boundary_tag(second_chunk);

    if (cache_table_set(second_chunk)) {    // add second_chunk to cache_table
        fprintf(stderr, "Unable to add chunk to cache_table in split_chun()");
        return NULL;
    };

    return first_chunk;
}

t_chunk* allocate_huge_chunk(size_t size) {
    // Allocate a huge chunk
    t_chunk* huge_chunk = (t_chunk*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (huge_chunk == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    huge_chunk->size = size;
    return huge_chunk;
}

static t_chunk* merge_chunks(t_chunk* value_1, t_chunk* value_2) {
    // remove t_chunks from cache_table
    if (value_1->bk) value_1->bk->fd = value_1->fd;
    if (value_1->fd) value_1->fd->bk = value_1->bk;
    if (value_2->bk) value_2->bk->fd = value_2->fd;
    if (value_2->fd) value_2->fd->bk = value_2->bk;
    printf("value_1->size before merge: %zu", value_1->size);
    value_1->size += value_2->size;
    printf("value_1->size after merge: %zu", value_1->size);
    write_boundary_tag(value_1);

    return value_1;
}

static t_chunk* try_merge(t_chunk* value) {
    // coalescing algo
    int flag = 0;
    t_chunk* prev = PREV_CHUNK(value, PREV_SIZE(value));
    t_chunk* next = NEXT_CHUNK(value); 

    if (!IS_IN_USE(prev)) flag += 1;
    if (!IS_IN_USE(next)) flag += 2;

    switch (flag) {
    case 1: return merge_chunks(prev, value);
    case 2: return merge_chunks(value, next);
    case 3: return merge_chunks(merge_chunks(prev, value), next);
    default: break;
    }

    return value;
}

void free_chunk(void* ptr, size_t size) {
    UNUSED(size);
    t_chunk* value = (t_chunk*)((char*)ptr - sizeof(size_t));
    value->bk = NULL;
    value->fd = NULL;
    SET_FREE(value);
    value = try_merge(value);

    // add to unsorted_cache
    t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
    value->fd = unsorted_chunk;
    g_pagemap->frontend_cache->unsorted_cache = value;
}

void free_huge_chunk(void* ptr, size_t size) {
    if (munmap(ptr, size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
}