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

char *get_date(time_t *__timer);
void format(char *timer_c);
unsigned int set_format(char *format);

#endif