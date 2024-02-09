#include "hash_table.h"

uint64_t my_hash_func(char *name, size_t length) {
    uint64_t hash_value = 0;
    for (int i = 0; i < length; i++) {
        hash_value += name[i];
        hash_value = hash_value * name[i];
    }

    return hash_value;
}

int main(int ac, char** av) {
    if (ac < 3) {
        printf("usage: ");
        return EXIT_FAILURE;
    }


    const int tablesize = (1<<20);
 
    hash_table *table = hash_table_create(tablesize, my_hash_func);

    return EXIT_SUCCESS;
}