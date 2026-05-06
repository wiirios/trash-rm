#ifndef BIN_HEADER
#define BIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "timestamp.h"

#ifdef __linux__
#include <sys/stat.h>
#include <dirent.h>
#include <regex.h>
#include <unistd.h>

#define GetCurrentDir getcwd
#endif

#define MAX_PATH_SIZE 4096
#define MAX_FILE_NAME 255
#define MAX_BUFFER_SIZE 1024

void move(const char *dest, const char *src, const char *file_name, unsigned char flag);
void move_file_match_pattern(const char *pattern);
void clean();

int list();
int check_if_file_exist(DIR *dir, const char *file_name);
int get_size(const char *file_name, size_t *out_size);


#endif