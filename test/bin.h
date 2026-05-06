#ifndef BIN_HEADER
#define BIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"

#ifdef __linux__
#include <sys/stat.h>
#include <dirent.h>
#include <regex.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#ifndef DEBUG
#define DEBUG 1
#define MAX_BUFFER_SIZE_DEBUG 255
#define MAX_FILE_NAME 255
#define MAX_BIN_PATH_NAME 8192 

#if defined(DEBUG)
    void move_debug(const char *dest, const char *src, const char *file_name, unsigned char flag);
    void move_file_match_pattern(const char *pattern);
    void clean();
    void remove_by_name_debug(const char *file_name);
    int list_debug();
    void recover_debug(const char *file_name);
#else
    void move_release(const char *src, const char *dest, FILE *file);
#endif

int check_if_file_exist(DIR *dir, const char *file_name);
int get_size(const char *file_name, size_t *out_size);

#endif
#endif