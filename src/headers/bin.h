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

/**
 * @brief Move a file or directory to the trash bin.
 *
 * @param dest Destination directory path, usually the trash folder.
 * @param src Source directory path containing the item.
 * @param file_name Name of the file or directory to move.
 * @param flag If 1, write an entry to list.txt for this trashed file. If 0,
 *        the move is being done during recovery, so do not add it to list.txt
 *        because the file is leaving the trash.
 * @note This flag behavior still needs to be fully implemented in the move()
 *       function.
 */
void move(const char *dest, const char *src, const char *file_name, unsigned char flag);

/**
 * @brief Move files matching a regular expression pattern to the trash bin.
 *
 * @param pattern Regular expression pattern used to select files.
 */
void move_file_match_pattern(const char *pattern);

/**
 * @brief Clean the trash bin by removing trashed items.
 */
void clean();

/**
 * @brief Remove an item from the trash bin by its file name.
 *
 * @param file_name Name of the trashed file to remove permanently.
 */
void remove_by_name_from_bin(const char *file_name);

/**
 * @brief Recover a file from the trash bin back to its original location.
 *
 * @param file_name Name of the file to recover.
 */
void recover(const char *file_name);

/**
 * @brief Show all items in trash bin.
 *
 * @return 1 if successful
 */
int list();

/**
 * @brief Check if a file with the given name exists in the opened directory.
 *
 * @param dir Pointer to an open DIR stream.
 * @param file_name Name of the file to check.
 * @return 1 if the file exists, 0 if it does not.
 */
int check_if_file_exist(DIR *dir, const char *file_name);

/**
 * @brief Get the file size for the given file path.
 *
 * @param file_name Path of the file to inspect.
 * @param out_size Pointer to size_t where the file size will be stored.
 * @return 0 on success, or a negative error code on failure.
 */
int get_size(const char *file_name, size_t *out_size);

#endif