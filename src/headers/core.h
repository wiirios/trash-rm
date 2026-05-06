#ifndef CORE_HEADER
#define CORE_HEADER

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#endif

void error(const char *msg);
void get_origin_path(char *buffer, size_t size);
void create_bin(const char *bin_path);

#endif