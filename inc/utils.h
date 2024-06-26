#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define UNUSED(x) (void)(x)

void log_info(const char* message);
void log_error(const char *error);
void log_heap();
void custom_exit(const char *error);

void system_settings();
void check_system_pointer();
size_t check_system_size_t();

int get_fpage_index(size_t nbr);
size_t round_up_to_next(size_t number);

char* my_strcpy(char* dst, char* src);
int my_strlen(char* str);

#endif