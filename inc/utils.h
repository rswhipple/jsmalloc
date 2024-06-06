#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define UNUSED(x) (void)(x)

void log_info(const char* message);
void log_span(t_span* span);

void system_settings();
void check_system_pointer();
size_t check_system_size_t();

#endif