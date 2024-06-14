#include "../../inc/main.h" 

/*
void print_chunks() {
    t_block *current_block = (t_block *)BLOCK_SHIFT(heap);
    while (current_block != NULL) {
        printf("block size: %zu\n", current_block->data_size);
        current_block = current_block->next;
    }
}
*/

void write_boundary_tag(t_chunk* chunk) {
    size_t* boundary_tag = (size_t*)((char*)chunk + CHUNK_SIZE(chunk) - sizeof(size_t));
    *boundary_tag = chunk->size;
}

t_chunk* create_top_chunk(t_page* page) {
    t_chunk* chunk = (t_chunk*)PAGE_SHIFT(page);
    SET_CHUNK_SIZE(chunk, page->memory);
    SET_FREE(chunk);
    chunk->fd = NULL;
    chunk->bk = NULL;
    write_boundary_tag(chunk);
    page->top_chunk = chunk;

    // log_info("creating top chunk");
    // printf("chunk pointer: %p\n", chunk);
    // printf("chunk's data pointer (same memory location as *fd): %p\n", chunk->fd);
    // printf("chunk size: %zu\n", chunk->size);
    // printf("sizeof(t_chunk): %zu\n", sizeof(t_chunk));
    // TODO: confirm this is correct?

    return chunk;
}

// input parameters are t_chunk *chunk and size_t chunk_size
t_chunk* split_chunk(t_chunk* chunk, size_t size) {
    if (chunk->size <= size) {
        fprintf(stderr, "Invalid split size: %zu (chunk size: %zu)\n", size, chunk->size);
        exit(EXIT_FAILURE);
    }

    // Placeholder variables
    t_chunk* temp = chunk->fd;
    size_t initial_chunk_size = chunk->size;

    // Update the original chunk's size, free status, next pointer & boundary_tag
    t_chunk* first_chunk = chunk;
    SET_CHUNK_SIZE(first_chunk, size);
    SET_IN_USE(first_chunk);
    first_chunk->fd = NULL;
    first_chunk->bk = NULL;
    write_boundary_tag(chunk);

    // Create the second chunk immediately after the first chunk
    t_chunk* second_chunk = (t_chunk*)MEMORY_SHIFT(CHUNK_SHIFT(chunk), size);
    SET_CHUNK_SIZE(second_chunk, initial_chunk_size - size);
    SET_FREE(second_chunk);
    second_chunk->bk = chunk;
    second_chunk->fd = temp;
    write_boundary_tag(second_chunk);

    // Return the first chunk
    return first_chunk;
}

t_chunk* allocate_huge_chunk(size_t size) {
    // Allocate a huge chunk
    t_chunk* huge_chunk = (t_chunk*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (huge_chunk == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    huge_chunk->size = size;
    return huge_chunk;
}

void free_chunk(void* ptr, size_t size) {
    t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
    // check for other free chunks and write coalescing algp

    // cast the memory space back into a chunk, set free and add to unsorted list
    t_chunk* chunk = (t_chunk*)((char*)ptr - sizeof(size_t));
    UNUSED(unsorted_chunk);
    UNUSED(chunk);
    UNUSED(size);
}