#include "../inc/main.h"

t_chunk** create_cache_table(t_cache* cache) {
    log_info("creating cache_table");
    t_chunk** cache_table = (t_chunk**)MEMORY_SHIFT(cache, (cache->fcache_size * sizeof(t_tiny_chunk*)));
    printf("cache_table pointer: %p\n", cache_table);

    void* last_byte = (void*)MEMORY_SHIFT(cache_table, (NUM_BINS * sizeof(t_chunk*)));
    printf("cache_table end: %p\n", last_byte);
    return cache_table;
}

size_t round_up_to_next(size_t number) {
    size_t list[] = { 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168,
    176, 184, 192, 200, 208, 216, 224, 232, 240, 248, 256, 264, 272, 280, 288,
    296, 304, 312, 320, 336, 368, 400, 432, 464, 496, 528, 560, 592, 624, 656,
    688, 720, 752, 784, 816, 848, 880, 912, 944, 976, 1008, 1040, 1072, 1104,
    1136, 1168, 1200, 1232, 1264, 1296, 1328, 1360, 1392, 1424, 1456, 1488,
    1520, 1552, 1584, 1616, 1648, 1680, 1712, 1744, 1776, 1808, 1840, 1872,
    1904, 1936, 1968, 2000, 2032, 2064, 2096, 2128, 2160, 2192, 2224, 2256,
    2288, 2320, 2352, 2384, 2416, 2448, 2480, 2512, 2544, 2576, 2608, 2640,
    2672, 2704, 2736, 2768, 2800, 2832, 2864, 2896, 2928, 2960, 2992, 3024,
    3056, 3088, 3120, 3152, 3184, 3216, 3248, 3280, 3312, 3344, 3376, 3408,
    3440, 3472, 3504, 3536, 3568, 3600, 3632, 3664, 3696, 3728, 3760, 3792,
    3824, 3856, 3888, 3920, 3952, 3984, 4016, 4048 };

    // Iterate through the list
    for (int i = 0; i < NUM_BINS; i++) {
        // If the current list element is greater than or equal to the number
        if (list[i] >= number) {
            return list[i];
        }
    }
    // If no larger or equal number is found, return the largest number in the list
    return list[NUM_BINS - 1]; // Assuming the list is sorted in ascending order
}

unsigned int my_hash_function(size_t data_size, uint32_t table_size) {
    double A = (sqrt(5) - 1) / 2; // Fractional part of the golden ratio
    return ((unsigned int)(table_size * (data_size * A - (int)(data_size * A)))) % table_size;
}

// t_cache_table* create_cache_table(t_cache* cache, uint32_t size, hash_function* hf) {
//     t_cache_table* ht = (t_cache_table*)MEMORY_SHIFT(cache, (cache->fcache_size * sizeof(t_tiny_chunk)));
//     ht->size = size;
//     ht->hash = hf;

//     ht->elements = (t_chunk**)MEMORY_SHIFT(heap, sizeof(t_cache_table) + (sizeof(t_chunk*) * MAX_BLOCKS));

//     size_t elements_size = size * sizeof(t_chunk*);
//     ht->elements = mmap(NULL, elements_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

//     if (ht->elements == MAP_FAILED) {
//         perror("mmap failed");
//         munmap(ht, sizeof(t_cache_table)); // Clean up the previously mapped memory
//         return NULL;
//     }

//     // Initialize hashtable slots to NULL
//     for (uint32_t i = 0; i < size; i++) {
//         ht->elements[i] = NULL;
//     }

//     return ht;
// }

// void cache_table_print(t_cache_table* ht) {
//     printf("Start Table\n");
//     for (uint32_t i = 0; i < ht->size; i++) {
//         if (ht->elements[i] == NULL) {
//             printf("\t%u\t---\n", i);
//         }
//         else {
//             printf("\t%u\t\n", i);
//             t_block* tmp = ht->elements[i];
//             while (tmp) {
//                 // printf("\"%TODO\"(%lu) - ", tmp->data_size);
//                 tmp = tmp->next;
//             }
//         }
//     }
// }

// bool cache_table_insert(t_heap** heap, t_cache_table* ht, size_t size) {
//     if (ht == NULL || size == 0) return false;
//     size_t index = ht->hash(size, ht->size);;

//     if (cache_table_allocate(ht, size)) return false;

//     // create new entry
//     t_block* block = create_block(*heap, size);
//     printf("block: %zu\n", block->data_size);
//     block->prev = NULL;
//     block->next = NULL;
//     block->data_size = size;
//     block->freed = false;

//     // TODO create an object to allocate the memory space
//     // block->object = 

//     (*heap)->block_count++;
//     (*heap)->free_size -= size;

//     // insert entry
//     ht->elements[index]->prev = block;
//     block->next = ht->elements[index];
//     ht->elements[index] = block;
//     return true;
// }

// void* cache_table_allocate(t_cache_table* ht, size_t key) {
//     if (key == 0 || ht == NULL) return false;
//     size_t index = ht->hash(key, ht->size);     // TODO: make sure this is the correct function pointer

//     t_block* tmp = ht->elements[index];
//     while (tmp && !tmp->freed) {
//         tmp = tmp->next;
//     }
//     if (!tmp) return NULL;  // This means we need to allocate more memory
//     return &tmp->data_size;
// }

// void* cache_table_deallocate(t_block* block) {
//     if (!block) return NULL;

//     // size_t index = cache_table_index(ht, key);

//     // t_block *tmp = ht->elements[index];
//     // t_block *prev = NULL;
//     // while (tmp && strcmp(tmp->key, key) != 0) {
//     //     prev = tmp;
//     //     tmp = tmp->next;
//     // }
//     // if (!tmp) return NULL;
//     // if (!prev) {
//     //     // deallocating the head of the list
//     //     // change this to making the block free again
//     //     ht->elements[index] = tmp->next;
//     // } else {
//     //     // deallocating from the middle 
//     //     prev->next = tmp->next;
//     // }
//     // void *result = tmp->data_size;
//     // free(tmp);

//     // return result;
//     return 0;
// }



