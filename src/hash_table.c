#include "../inc/hash_table.h"
#include <sys/mman.h>
#include <unistd.h>

unsigned int my_hash_function(size_t data_size, uint32_t table_size) {
    double A = (sqrt(5) - 1) / 2; // Fractional part of the golden ratio
    return ((unsigned int)(table_size * (data_size * A - (int)(data_size * A)))) % table_size;
}

t_hash* hash_table_create(t_heap* heap, uint32_t size, hash_function* hf) {
    t_hash* ht = (t_hash*)HASH_SHIFT((char*)heap);;
    ht->size = size;
    ht->hash = hf;

    // TODO: calculate the number of indexes possible
    ht->elements = (t_block**)MEMORY_SHIFT(heap, sizeof(t_hash) + (sizeof(t_block*) * MAX_BLOCKS));

    size_t elements_size = size * sizeof(t_block*);
    ht->elements = mmap(NULL, elements_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ht->elements == MAP_FAILED) {
        perror("mmap failed");
        munmap(ht, sizeof(t_hash)); // Clean up the previously mapped memory
        return NULL;
    }

    // Initialize hashtable slots to NULL
    for (uint32_t i = 0; i < size; i++) {
        ht->elements[i] = NULL;
    }

    return ht;
}

void hash_table_print(t_hash* ht) {
    printf("Start Table\n");
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->elements[i] == NULL) {
            printf("\t%u\t---\n", i);
        }
        else {
            printf("\t%u\t\n", i);
            t_block* tmp = ht->elements[i];
            while (tmp) {
                // printf("\"%TODO\"(%lu) - ", tmp->data_size);
                tmp = tmp->next;
            }
        }
    }
}

bool hash_table_insert(t_heap** heap, t_hash* ht, size_t size) {
    if (ht == NULL || size == 0) return false;
    size_t index = ht->hash(size, ht->size);;

    if (hash_table_allocate(ht, size)) return false;

    // create new entry
    t_block* block = create_block(*heap, size);
    printf("block: %zu\n", block->data_size);
    block->prev = NULL;
    block->next = NULL;
    block->data_size = size;
    block->freed = false;

    // TODO create an object to allocate the memory space
    // block->object = 

    (*heap)->block_count++;
    (*heap)->free_size -= size;

    // insert entry
    ht->elements[index]->prev = block;
    block->next = ht->elements[index];
    ht->elements[index] = block;
    return true;
}

void* hash_table_allocate(t_hash* ht, size_t key) {
    if (key == 0 || ht == NULL) return false;
    size_t index = ht->hash(key, ht->size);     // TODO: make sure this is the correct function pointer

    t_block* tmp = ht->elements[index];
    while (tmp && !tmp->freed) {
        tmp = tmp->next;
    }
    if (!tmp) return NULL;  // This means we need to allocate more memory
    return &tmp->data_size;
}

void* hash_table_deallocate(t_block* block) {
    if (!block) return NULL;

    // size_t index = hash_table_index(ht, key);

    // t_block *tmp = ht->elements[index];
    // t_block *prev = NULL;
    // while (tmp && strcmp(tmp->key, key) != 0) {
    //     prev = tmp;
    //     tmp = tmp->next;
    // }
    // if (!tmp) return NULL;
    // if (!prev) {
    //     // deallocating the head of the list
    //     // change this to making the block free again
    //     ht->elements[index] = tmp->next;
    // } else {
    //     // deallocating from the middle 
    //     prev->next = tmp->next;
    // }
    // void *result = tmp->data_size;
    // free(tmp);

    // return result;
    return 0;
}



