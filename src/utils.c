#include "../inc/main.h"


void log_info(const char* message) {
    printf("\n=====%s=====\n", message);
}

/*
system_settings(): Sets the global variable min_chunk_size.
min_chunk_size affects the minimum t_tiny_chunk size and is dependant on whether
the OS uses 4 byte or 8 byte pointers.
*/

void system_settings() {
    check_system_pointer();
    if (pointer_size == 4) {
        min_chunk_size = 16;
        log_info("Minimum chunk size is 8 bytes (4 free).");
    }
    else {
        min_chunk_size = 16;
        log_info("Minimum chunk size is 16 bytes (8 free).");
    }
}

void check_system_pointer() {
    pointer_size = sizeof(void*);
    if (pointer_size == 4) {
        log_info("System has 4-byte pointers.");
    }
    else if (pointer_size == 8) {
        log_info("System has 8-byte pointers.");
    }
    else {
        printf("Unexpected pointer size: %zu bytes\n", pointer_size);
        // add error exit
    }
}

size_t check_system_size_t() {
    size_t size_t_size = sizeof(size_t);

    if (size_t_size == 4) {
        log_info("System has 4-byte size_t.");
    }
    else if (size_t_size == 8) {
        log_info("System has 8-byte size_t.");
    }
    else {
        printf("Unexpected size_t size: %zu bytes\n", size_t_size);
        // add error exit
    }

    return size_t_size;
}

int get_fpage_index(size_t nbr) {
    int num_pages = g_pagemap->frontend_cache->fcache_size;
    int i;
    size_t list[] = { 8, 16, 24, 32, 40, 48, 56, 64 };
    int list_len = 8;

    // Iterate through the list
    for (i = 0; i < list_len; i++) {
        // If the current list element is greater than or equal to the number
        if (list[i] >= nbr) {
            break;
        }
    }

    // TODO: If no larger or equal number is found, throw error

    // Logic if min_chunk_size is 16
    if (num_pages == 7 && i > 0) return i - 1;
    else return i;
}

size_t round_up_to_next(size_t number) {
    // add CHUNK_OVERHEAD before rounding to hold space for boundary markers
    number += CHUNK_OVERHEAD;
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


char* my_strcpy(char* dst, char* src)
{

    int i;
    int length = my_strlen(src);

    for (i = 0; i < length; i++) {
        dst[i] = src[i]; 
    }

    dst[i] = '\0';

    return dst;
}


int my_strlen(char* str) 
{
    int i = 0;

    do {
        i += 1;
    } while (str[i] != '\0');

    return i;
}

