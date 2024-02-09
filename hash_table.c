#include "hash_table.h"
#include <stdio.h>
#include <string.h>

struct entry {
    char *key;
    void *object;
    struct entry *next;
};

struct hash_table_s {
    uint32_t size;
    hash_function *hash;
    entry **elements;
};

static size_t hash_table_index(hash_table *ht, const char *key) {
    size_t result = (ht->hash(key, strlen(key)) % ht->size);
    return result;
}

hash_table *hash_table_create(uint32_t size, hash_function *hf) {
    hash_table *ht = malloc(sizeof(*ht));
    ht->size = size;
    ht->hash = hf;
    // note: calloc seroes out the memory, which is why we use it
    ht->elements = calloc(sizeof(entry*), ht->size);
    return ht;
}

void hash_table_destroy(hash_table *ht) {
    // cleanup individual elements
    for (uint32_t i = 0; i < ht->size; i++) {
        while (ht->elements[i]) {
            entry *tmp = ht->elements[i];
            ht->elements[i] = ht->elements[i]->next;
            free(tmp->key);
            free(tmp->object); // make more general
            free(tmp);
        }
    }
    free(ht->elements);
    free(ht);
}

void hash_table_print(hash_table *ht) {
    printf("Start Table\n");
    for (int i = 0; i < ht->size; i++) {
        if (ht->elements[i] == NULL) {
            printf("\t%i\t---\n", i);
        } else {
            printf("\t%i\t\n", i);
            entry *tmp = ht->elements[i];
            while (tmp) {
                printf("\"%s\"(%p) - ", tmp->key, tmp->object);
                tmp = tmp->next;
            }
        }
    }
}

bool hash_table_insert(hash_table *ht, const char *key, void *value) {
    if (key == NULL || value == NULL) return false;
    size_t index = hash_table_index(ht, key);

    if (hash_table_allocate(ht, key)) return false;

    // create new entry
    entry *e = malloc(sizeof(entry));
    e->object = value;
    e->key = malloc(strlen(key) + 1);
    strcpy(e->key, key);
    // can replace last 2 lines with strdup(key)

    // insert entry
    e->next = ht->elements[index];
    ht->elements[index] = e;
    return true;
}

void *hash_table_allocate(hash_table *ht, const char *key) {
    if (key == NULL || ht == NULL) return false;
    size_t index = hash_table_index(ht, key);

    entry *tmp = ht->elements[index];
    while (tmp && strcmp(tmp->key, key) != 0) {
        tmp = tmp->next;
    }
    if (!tmp) return NULL;
    return tmp->object;
}

void *hash_table_deallocate(hash_table *ht, const char *key) {
    if (key == NULL || ht == NULL) return false;
    size_t index = hash_table_index(ht, key);

    entry *tmp = ht->elements[index];
    entry *prev = NULL;
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
    void *result = tmp->object;
    free(tmp);

    return result;
}


unsigned int hashFunction(int key, int tableSize) {
    double A = (sqrt(5) - 1) / 2; // Fractional part of the golden ratio
    return (unsigned int)(tableSize * (key * A - (int)(key * A)));
}

