#ifndef CORE_HEADER
#define CORE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define TIMESTAMP "DDMMYYYY"

void error(const char *msg);
void get_bin_path(char *buffer, size_t size);
void create_bin(const char *bin_path);

#endif