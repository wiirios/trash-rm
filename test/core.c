#include "core.h"

void error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void get_bin_path(char *buffer, size_t size) {
    snprintf(buffer, size, "%s/.local/share/trash-rm/", getenv("HOME"));
}

void create_bin(const char *bin_path) {
    if (mkdir(bin_path, 0755) == -1 && errno != EEXIST) {
        error("Cannot create bin folder");
        exit(EXIT_FAILURE);
    }
}
