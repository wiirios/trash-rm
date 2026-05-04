#include "bin.h"

#if DEBUG
void move_debug(const char *file_name, unsigned char flag) {
    char buff[MAX_FILE_NAME];
    GetCurrentDir(buff, FILENAME_MAX);

    DIR *current_dir = opendir(buff);

    if (check_if_file_exist(current_dir, file_name)) goto found;

    closedir(current_dir);
    error("File not found");

    found:
    size_t size_file;
    if (get_size(file_name, &size_file) != 0) error("Cannot get file size");

    char *BUFFER = malloc(size_file + 1);
    if (!BUFFER) error("Error allocating memory for Buffer");

    char BUFFER_PATH[MAX_BIN_PATH_NAME];
    get_bin_path(BUFFER_PATH, sizeof(BUFFER_PATH));

    size_t buffer_l = strlen(BUFFER_PATH);

    strcat(BUFFER_PATH, "trash/");
    
    DIR *dir = opendir(BUFFER_PATH);
    if (!dir) {
        closedir(dir);
        error("Cannot access the bin folder");
    }

    rewinddir(dir);
    char q;
    if (check_if_file_exist(dir, file_name)) {
        while (1) {
            printf("File %s already exists, do you want to overwrite it?\nYes [Y]\nNo [N]\n", file_name);
            scanf("%c", &q);

            if (q == 'Y' || q == 'y' || q == 'N' || q == 'n') break;
        }
        
        if (q == 'N' || q == 'n') error("The file was not overwritten");
    }

    snprintf(BUFFER, size_file, "%s%s", BUFFER_PATH, file_name);
    if ((rename(file_name, BUFFER)) == 0) flag ? printf("File %s move succesfully\n", file_name) : printf("File move succesfully\n");
    else {
        error("Error moving file");
    }
    ;
    
    BUFFER_PATH[buffer_l] = '\0';
    strcat(BUFFER_PATH, "list.txt");
    FILE *list = fopen(BUFFER_PATH, "a");
    if (!list) error("It was not possible to open list.txt");

    fprintf(list, "%s | %s | %s\n", "TIME", file_name, buff);

    fclose(list);
    closedir(current_dir);
    closedir(dir);
    free(BUFFER);
}

int list_debug() {
    FILE *list_ = fopen("../assets/list.txt", "r");
    if (!list_) {
        fclose(list_);
        error("It was not possible to open list.txt");
    }

    char line[MAX_BUFFER_SIZE_DEBUG];

    while (fgets(line, sizeof(line), list_)) {
        printf("%s", line);
    }

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
            move_debug(d->d_name, 1);
        }
        else if (status == REG_NOMATCH) {
            continue;
        }
    }

    if (!called) printf("No file found with pattern %s \n", pattern);

    regfree(&regex);
    closedir(current_dir);
}

int check_if_file_exist(DIR *dir, const char *file_name) {
    struct dirent *d;
    while ((d = readdir(dir)) != NULL) if (strcmp(d->d_name, file_name) == 0) return 1; 
        
    return 0;
}

void clean() {
    int result = list_debug();

    if (!result) return;
    
    char BUFFER_PATH[MAX_BIN_PATH_NAME];
    get_bin_path(BUFFER_PATH, sizeof(BUFFER_PATH));
    
    DIR *dir = opendir(BUFFER_PATH);
    struct dirent *d;
    char q;

    while (1) {
        printf("Are you sure you want to remove all files from the bin?\nYes [Y]\nNo [N]\n");
        scanf("%c", &q);

        if (q == 'Y' || q == 'y' || q == 'N' || q == 'n') break;
    }

    if (q == 'N' || q == 'n') error("The bin can was not emptied.");

    while ((d = readdir(dir)) != NULL) if (d->d_type == 8) {
        char file_path[MAX_BIN_PATH_NAME];
        int written = snprintf(file_path, sizeof(file_path), "%s%s", BUFFER_PATH, d->d_name);
        
        if (written < 0 || (size_t)written >= sizeof(file_path)) error("Path too long");

        remove(file_path);
    }
}

void remove_by_name_debug(const char *file_name) {
    char BUFFER_PATH[MAX_BIN_PATH_NAME];
    get_bin_path(BUFFER_PATH, sizeof(BUFFER_PATH));
    
    DIR *dir = opendir(BUFFER_PATH);
    
    if (!dir) {
        closedir(dir);
        error("Cannot access the bin folder");
    } 

    if (check_if_file_exist(dir, file_name)) {
        char file_path[MAX_BIN_PATH_NAME];
        snprintf(file_path, sizeof(file_path), "%s%s", BUFFER_PATH, file_name); 
            
        remove(file_path);
    } else {
        closedir(dir);
        error("File not found");
    }

    printf("File %s move succesfully\n", file_name);
    closedir(dir);
}

void recover_debug(const char *file_name) {
    char BUFFER_PATH[MAX_BIN_PATH_NAME];
    get_bin_path(BUFFER_PATH, sizeof(BUFFER_PATH));
    
    DIR *dir = opendir(BUFFER_PATH);
    
    if (!dir) {
        closedir(dir);
        error("Cannot access the bin folder");
    }

    if (check_if_file_exist(dir, file_name)) {
        FILE *list = fopen("../assets/list.txt", "r");
        
        if (!list) {
            fclose(list);
            error("It was not possible to open list.txt");
        }

        int c, i, j, jump, can_get_path;
        jump = i = j = can_get_path = 0;

        char BUFFER_PATH[1024];
        char BUFFER_NAME[1024];

        while ((c = fgetc(list)) != EOF) {
            if (c == '|') jump++;
       
            if (jump == 1 && c != '|' && c != ' ') {
                BUFFER_NAME[i++] = c;
                BUFFER_NAME[i] = '\0';
            }

            if (jump == 2 && !can_get_path) {
                if (strcmp(BUFFER_NAME, file_name) == 0) {
                    can_get_path = 1;
                    BUFFER_NAME[i] = '\0';
                }
            }

            if (can_get_path) {
                if (c != '|' && c != ' ') {
                    BUFFER_PATH[j] = c;
                    j++;
                }
            }

            if (can_get_path && c == '\n') break;

            if (c == '\n') {
                i = j = jump = 0;
                memset(BUFFER_NAME, 0 , sizeof(BUFFER_NAME));
            }
        }
        printf("%s\n", BUFFER_NAME);
        printf("%s\n", BUFFER_PATH);
        closedir(dir);
        fclose(list);
    } else {
        closedir(dir);
        error("File not found");
    }

}

#else
void move_release(const char *src, const char *dest, FILE *file) {
    char *BUFFER = malloc(get_size(src) + 1);

    if (!BUFFER) {
        perror("error allocating memory for BUFFER");
        exit(EXIT_FAILURE);
    }

    fclose(file);

    if ((rename(src, dest)) == 0) printf("File moved succesfully");
    else {
        perror("error moving file");
        exit(EXIT_FAILURE);
    }
}
#endif

int get_size(const char *file_name, size_t *out_size) {
    struct stat st;

    if ((stat(file_name, &st)) != 0) return -1;

    if ((size_t) st.st_size > __SIZE_MAX__ || st.st_size < 0) return -1;

    *out_size = (size_t) st.st_size;

    return 0;
}
