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
    chunk->size = page->memory;
    chunk->fd = NULL;
    chunk->bk = NULL;
    // char* data = (char*)MEMORY_SHIFT(chunk, sizeof(size_t));
    SET_FREE(chunk);
    write_boundary_tag(chunk);
    // log_info("creating top chunk");
    // printf("chunk pointer: %p\n", chunk);
    // printf("chunk's data pointer (same memory location as *fd): %p\n", chunk->fd);
    // printf("chunk size: %zu\n", chunk->size);
    // printf("sizeof(t_chunk): %zu\n", sizeof(t_chunk));
    page->top_chunk = chunk;
    // TODO: confirm this is correct?
    return chunk;
}

// input parameters are t_chunk *chunk and size_t chunk_size
t_chunk* split_chunk(t_chunk* chunk, size_t size) {
    if (chunk->size <= size) {
        fprintf(stderr, "Invalid split size: %zu (chunk size: %zu)\n", size, chunk->size);
        exit(EXIT_FAILURE);
    }
    t_chunk* temp = chunk->fd;
    size_t initial_chunk_size = chunk->size;

    // Update the original chunk's size, free status, next pointer & boundary_tag
    t_chunk* first_chunk = chunk;
    first_chunk->size = size;
    SET_FREE(first_chunk);
    write_boundary_tag(chunk);

    // Create the second chunk immediately after the first chunk
    t_chunk* second_chunk = (t_chunk*)MEMORY_SHIFT(CHUNK_SHIFT(chunk), size);
    second_chunk->size = initial_chunk_size - size;
    second_chunk->bk = chunk;
    second_chunk->fd = temp;
    SET_FREE(second_chunk);
    write_boundary_tag(second_chunk);

    // Update the original chunk's fd pointer
    chunk->fd = second_chunk;

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
