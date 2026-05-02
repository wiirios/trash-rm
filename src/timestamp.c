#include "headers/timestamp.h"

typedef struct
{
    char full_date[TIMESTAMP_LENGTH];
    char format[MAX_FORMAT_LENGTH];
    unsigned int is_formated;
} timer_s;

static timer_s timer_str;

char *get_date(time_t *__timer) {
    if (timer_str.is_formated == 0) error("format the date using unsigned int set_format(char *format)");

    format(ctime(__timer));
    return timer_str.full_date;
}

unsigned int set_format(char *format) {
    timer_str.is_formated = 0;

    if (strlen(format) != MAX_FORMAT_LENGTH - 1) error("the format size is larger or smaller than allowed.");

    timer_str.is_formated = 1;
    strcpy(timer_str.format, format);
    return 1;
}

void format(char *timer_c) {    
    char day[3] = {timer_c[8], timer_c[9], '\0'};
    char year[5] = {timer_c[20], timer_c[21], timer_c[22], timer_c[23], '\0'};
    char hours_minutes_seconds[9] = {timer_c[11], timer_c[12], timer_c[13], timer_c[14], timer_c[15], timer_c[16], timer_c[17], timer_c[18], '\0'}; 
    char month[3];

    char *ptr = timer_c+4;

    const char mon_name[][12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    int i, m;
    i = 0;
    m = -1;

    for (; i < 12; i++) {
        if (strncmp(ptr, mon_name[i], 3) == 0) {
            m = i + 1;
            break;
        }
    }

    if (m == -1) {
        perror("Invalid month");
        exit(EXIT_FAILURE);
    }

    month[0] = '0';
    month[1] = m + '0';
    month[2] = '\0';

    if ((strcmp(timer_str.format, "DDMMYYYY")) == 0) snprintf(timer_str.full_date, sizeof(timer_str.full_date), "%s-%s-%s %s", day, month, year, hours_minutes_seconds);
    else if ((strcmp(timer_str.format, "DDYYYYMM")) == 0) snprintf(timer_str.full_date, sizeof(timer_str.full_date), "%s-%s-%s %s", day, year, month, hours_minutes_seconds);
    else if ((strcmp(timer_str.format, "MMDDYYYY")) == 0) snprintf(timer_str.full_date, sizeof(timer_str.full_date), "%s-%s-%s %s", month, day, year, hours_minutes_seconds);
    else if ((strcmp(timer_str.format, "MMYYYYDD")) == 0) snprintf(timer_str.full_date, sizeof(timer_str.full_date), "%s-%s-%s %s", month, year, day, hours_minutes_seconds);
    else if ((strcmp(timer_str.format, "YYYYDDMM")) == 0) snprintf(timer_str.full_date, sizeof(timer_str.full_date), "%s-%s-%s %s", year, day, month, hours_minutes_seconds);
    else if ((strcmp(timer_str.format, "YYYYMMDD")) == 0) snprintf(timer_str.full_date, sizeof(timer_str.full_date), "%s-%s-%s %s", year, month, day, hours_minutes_seconds);
    else error("Invalid timestamp format");
}
