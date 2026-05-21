#ifndef CORE_HEADER
#define CORE_HEADER

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#endif

/**
 * @brief Print an error message and terminate the program.
 *
 * @param msg Null-terminated error message string.
 */
void error(const char *msg);

/**
 * @brief Fill a buffer with the application origin path.
 *
 * @param buffer Destination buffer to receive the origin path.
 * @param size Size of the destination buffer.
 */
void get_origin_path(char *buffer, size_t size);

/**
 * @brief Create the trash bin directory if it does not exist.
 *
 * @param bin_path Path to the bin directory to create.
 */
void create_bin(const char *bin_path);

#endif