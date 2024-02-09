#include "hash_table.h"
#include "my_malloc.h"

struct hash_table_s {
    uint32_t size;
    hash_function *hash;
    t_block **elements;
};

static size_t hash_table_index(t_hash *ht, const char *key) {
    size_t result = (ht->hash(key, strlen(key)) % ht->size);
    return result;
}

t_hash *hash_table_create(t_heap *heap, uint32_t size, hash_function *hf) {
    t_hash *ht = HASH_SHIFT(heap);;
    ht->size = size;
    ht->hash = hf;
    // TODO: check if this works??
    ht->elements = BLOCK_SHIFT(heap) * ht->size;
    return ht;
}

void hash_table_destroy(t_hash *ht) {
    // cleanup individual elements
    for (uint32_t i = 0; i < ht->size; i++) {
        while (ht->elements[i]) {
            t_block *tmp = ht->elements[i];
            ht->elements[i] = ht->elements[i]->next;
            free(tmp->key);
            // free(tmp->object); // TODO: make it work for t_block
            free(tmp);
        }
    }
    free(ht->elements);
    free(ht);
}

void hash_table_print(t_hash *ht) {
    printf("Start Table\n");
    for (int i = 0; i < ht->size; i++) {
        if (ht->elements[i] == NULL) {
            printf("\t%i\t---\n", i);
        } else {
            printf("\t%i\t\n", i);
            t_block *tmp = ht->elements[i];
            while (tmp) {
                printf("\"%s\"(%lu) - ", tmp->key, tmp->data_size);
                tmp = tmp->next;
            }
        }
    }
}

bool hash_table_insert(t_heap **heap, t_hash *ht, const char *key, size_t size) {
    if (key == NULL || value == NULL) return false;
    size_t index = hash_table_index(ht, key);

    if (hash_table_allocate(ht, key)) return false;

    // create new entry
    t_block *b = create_block();
    printf("block: %zu\n", block->data_size);
    block->prev = NULL;
    block->next = NULL;
    block->data_size = size;
    block->freed = false;
    (*heap)->block_count++;
    (*heap)->free_size -= size;

    // TODO fix key and malloc
    b->key = malloc(strlen(key) + 1);
    strcpy(b->key, key);

    // insert entry
    ht->elements[index]->prev = b;
    b->next = ht->elements[index];
    ht->elements[index] = b;
    return true;
}

void *hash_table_allocate(t_hash *ht, const char *key) {
    if (key == NULL || ht == NULL) return false;
    size_t index = hash_table_index(ht, key);

    t_block *tmp = ht->elements[index];
    while (tmp && strcmp(tmp->key, key) != 0) {
        tmp = tmp->next;
    }
    if (!tmp) return NULL;
    return tmp->data_size;
}

void *hash_table_deallocate(t_hash *ht, const char *key) {
    if (key == NULL || ht == NULL) return false;
    size_t index = hash_table_index(ht, key);

    t_block *tmp = ht->elements[index];
    t_block *prev = NULL;
    while (tmp && strcmp(tmp->key, key) != 0) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (!tmp) return NULL;
    if (!prev) {
        // deallocating the head of the list
        // change this to making the block free again
        ht->elements[index] = tmp->next;
    } else {
        // deallocating from the middle 
        prev->next = tmp->next;
    }
    void *result = tmp->data_size;
    free(tmp);

    return result;
}


unsigned int my_hash_function(int key, int tableSize) {
    double A = (sqrt(5) - 1) / 2; // Fractional part of the golden ratio
    return (unsigned int)(tableSize * (key * A - (int)(key * A)));
}

