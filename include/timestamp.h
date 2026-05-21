#ifndef TIMESTAMP_HEADER
#define TIMESTAMP_HEADER

#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"

#define MAX_FORMAT_LENGTH 9
#define TIMESTAMP_LENGTH 20
#define TIMESTAMP_FORMAT "DDMMYYYY"

/**
 * @brief Return a formatted date string for the given timer value.
 *
 * @param __timer Pointer to a time_t value representing the time to format.
 * @return Pointer to a statically allocated string containing the formatted date.
 */
char *get_date(time_t *__timer);

/**
 * @brief Format the provided timestamp buffer using the current format.
 *
 * @param timer_c Buffer containing the raw timestamp string to format.
 */
void format(char *timer_c);

/**
 * @brief Set the timestamp format.
 *
 * @param format Format string to use for timestamp formatting.
 *               Must have exactly MAX_FORMAT_LENGTH - 1 characters.
 * @return 1 on success.
 * @note On invalid length the function calls error() and does not return.
 */
unsigned int set_format(char *format);

#endif