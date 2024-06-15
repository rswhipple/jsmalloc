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
    // log_info("splitting chunk");
    if (CHUNK_SIZE(chunk) <= size) {
        fprintf(stderr, "Invalid split size: %zu (chunk size: %zu)\n", size, chunk->size);
        exit(EXIT_FAILURE);
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
    UNUSED(size);
    t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
    // check for other free chunks and write coalescing algp

    // cast the memory space back into a chunk, set free and add to unsorted list
    t_chunk* chunk = (t_chunk*)((char*)ptr - sizeof(size_t));
    UNUSED(unsorted_chunk);
    UNUSED(chunk);
}

void free_huge_chunk(void* ptr, size_t size) {
    if (munmap(ptr, size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
}