#include "headers/bin.h"

void move(const char *file_name, unsigned char flag) {
    char buff[MAX_FILE_NAME];
    GetCurrentDir(buff, FILENAME_MAX);

    DIR *current_dir = opendir(buff);
    struct dirent *d;

    while ((d = readdir(current_dir)) != NULL) if (strcmp(d->d_name, file_name) == 0) goto found;

    closedir(current_dir);
    error("File not found");

    found:
    size_t size_file;
    if (get_size(file_name, &size_file) != 0) error("Cannot get file size");

    char *BUFFER = malloc(size_file + 1);
    if (!BUFFER) error("Error allocating memory for Buffer");

    DIR *dir = opendir(PATH_BIN);
    if (!dir) {
        closedir(dir);
        error("Cannot access the bin folder");
    }

    rewinddir(dir);
    char q;
    if (check_if_file_exist(d, dir, file_name) == 1) {
        while (1) {
            printf("File %s already exists, do you want to overwrite it?\nYes [Y]\nNo [N]\n", file_name);
            scanf("%c", &q);

            if (q == 'Y' || q == 'y' || q == 'N' || q == 'n') break;
        }
        
        if (q == 'N' || q == 'n') error("The file was not overwritten");
    }

    snprintf(BUFFER, size_file, "%s%s", PATH_BIN, file_name);
    if ((rename(file_name, BUFFER)) == 0) flag ? printf("File %s move succesfully\n", d->d_name) : printf("File move succesfully\n");
    else {
        error("Error moving file");
    }
    ;

    FILE *list_ = fopen("../assets/list.txt", "a");
    if (!list_) {
        fclose(list_);
        error("It was not possible to open list.txt");
    }

    time_t today = time(NULL);
    set_format(TIMESTAMP_FORMAT);

    fprintf(list_, "%s | %s\n", get_date(&today), file_name);

    fclose(list_);
    closedir(current_dir);
    closedir(dir);
    free(BUFFER);
}

int list() {
    FILE *list_ = fopen("../assets/list.txt", "r");
    if (!list_) {
        fclose(list_);
        error("It was not possible to open list.txt");
    }

    char line[MAX_BUFFER_SIZE];

    while (fgets(line, sizeof(line), list_)) printf("%s", line);

    fclose(list_);
    return 1;
}

void move_file_match_pattern(const char *pattern) {
    char buff[MAX_FILE_NAME];
    GetCurrentDir(buff, FILENAME_MAX);
    DIR *current_dir = opendir(buff);

    regex_t regex;
    int status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status) {
        regfree(&regex);
        error("Could not compile the regex");
    }

    if (!current_dir) {
        closedir(current_dir);
        error("Cannot access the bin folder");
    }
    struct dirent *d;
    int count, called;
    count = called = 0;

    while ((d = readdir(current_dir)) != NULL) ++count;

    if (count == 0) {
        regfree(&regex);
        closedir(current_dir);
        error("There's no file to remove");
    }
    rewinddir(current_dir);
    while ((d = readdir(current_dir)) != NULL) {
        status = regexec(&regex, d->d_name, 0, NULL, 0); 

        if (!status && d->d_type == 8) {
            if (!called) called += 1;
            move(d->d_name, 1);
        }
        else if (status == REG_NOMATCH) {
            continue;
        }
    }

    if (!called) printf("No file found with pattern %s \n", pattern);

    regfree(&regex);
    closedir(current_dir);
}

int check_if_file_exist(struct dirent *d, DIR *dir, const char *file_name) {
    while ((d = readdir(dir)) != NULL) if (strcmp(d->d_name, file_name) == 0) return 1; 
        
    return 0;
}

void clean() {
    int result = list();

    if (!result) return;

    DIR *dir = opendir(PATH_BIN);
    struct dirent *d;
    char q;

    while (1) {
        printf("Are you sure you want to remove all files from the bin?\nYes [Y]\nNo [N]\n");
        scanf("%c", &q);

        if (q == 'Y' || q == 'y' || q == 'N' || q == 'n') break;
    }

    if (q == 'N' || q == 'n') error("The bin can was not emptied.");

    while ((d = readdir(dir)) != NULL) if (d->d_type == 8) {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s%s", PATH_BIN, d->d_name);

        remove(file_path);
    }
}

int get_size(const char *file_name, size_t *out_size) {
    struct stat st;

    if ((stat(file_name, &st)) != 0) return -1;

    if ((size_t) st.st_size > __SIZE_MAX__ || st.st_size < 0) return -1;

    *out_size = (size_t) st.st_size;

    return 0;
}
