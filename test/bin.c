#include "bin.h"

#if DEBUG
void move_debug(const char *dest, const char *src, const char *file_name, unsigned char flag) {
    DIR *dest_ = opendir(dest);
    DIR *src_ = opendir(src);
    
    if (check_if_file_exist(src_, file_name)) goto found;

    closedir(src_);
    error("File not found");

    found:
    char src_path[MAX_BIN_PATH_NAME];
    char dest_path[MAX_BIN_PATH_NAME];

    int written = snprintf(src_path, sizeof(src_path), "%s%s%s", src, src[strlen(src) - 1] == '/' ? "" : "/", file_name);
    if (written < 0 || (size_t)written >= sizeof(src_path)) error("Path too long");

    written = snprintf(dest_path, sizeof(dest_path), "%s%s%s", dest, dest[strlen(dest) - 1] == '/' ? "" : "/", file_name);
    if (written < 0 || (size_t)written >= sizeof(dest_path)) error("Path too long");

    size_t size_file;
    if (get_size(src_path, &size_file) != 0) error("Cannot get file size");

    rewinddir(dest_);
    char q;
    if (check_if_file_exist(dest_, file_name)) {
        while (1) {
            printf("File %s already exists, do you want to overwrite it?\nYes [Y]\nNo [N]\n", file_name);
            scanf("%c", &q);

            if (q == 'Y' || q == 'y' || q == 'N' || q == 'n') break;
        }
        
        if (q == 'N' || q == 'n') error("The file was not overwritten");
    }
    
    if ((rename(src_path, dest_path)) == 0) printf("File %s move succesfully\n", file_name);
    else {
        error("Error moving file");
    }
    ;
    
    if (flag) {
        char dest_c[MAX_BIN_PATH_NAME];
        strcpy(dest_c, dest);
        dest_c[strlen(dest_c) - 6] = '\0';
        strcat(dest_c, "list.txt");
        FILE *list = fopen(dest_c, "a");
        
        printf("%s\n", dest_c);
        if (!list) {
            closedir(src_);
            closedir(dest_);
            error("It was not possible to open list.txt");
        }
        
        fprintf(list, "%s | %s | %s\n", "TIME", file_name, src);
        fclose(list);
    } else {
        remove_log_entry_debug(file_name);
    }

    closedir(src_);
    closedir(dest_);
}

int list_debug() {
    char list_txt[MAX_BIN_PATH_NAME];
    get_bin_path(list_txt, sizeof(list_txt));
    strcat(list_txt, "list.txt");
    
    FILE *list_ = fopen(list_txt, "r");
    if (!list_) error("It was not possible to open list.txt");

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
    
    char BUFFER_PATH_BIN[MAX_BIN_PATH_NAME];
    get_bin_path(BUFFER_PATH_BIN, sizeof(BUFFER_PATH_BIN));
    strcat(BUFFER_PATH_BIN, "/trash");

    while ((d = readdir(current_dir)) != NULL) {
        status = regexec(&regex, d->d_name, 0, NULL, 0); 

        if (!status && d->d_type == 8) {
            if (!called) called += 1;
            move_debug(BUFFER_PATH_BIN, buff , d->d_name, 0);
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
    
    strcat(BUFFER_PATH, "trash/");
    DIR *dir = opendir(BUFFER_PATH);
    struct dirent *d;
    char q;

    while (1) {
        printf("Are you sure you want to remove all files from the bin?\nYes [Y]\nNo [N]\n");
        scanf("%c", &q);

        if (q == 'Y' || q == 'y' || q == 'N' || q == 'n') break;
    }

    if (q == 'N' || q == 'n') error("The bin can was not emptied.");

    char BUFFER_FILE_PATH[MAX_BIN_PATH_NAME];
    while ((d = readdir(dir)) != NULL) if (d->d_type == 8) {
        int written = snprintf(BUFFER_FILE_PATH, sizeof(BUFFER_FILE_PATH), "%s%s", BUFFER_PATH, d->d_name);
        
        if (written < 0 || (size_t)written >= sizeof(BUFFER_FILE_PATH)) error("Path too long");
        
        remove(BUFFER_FILE_PATH);
        memset(BUFFER_FILE_PATH, 0 , sizeof(BUFFER_FILE_PATH));
    }
    
    BUFFER_PATH[strlen(BUFFER_PATH) - 6] = '\0';
    strcat(BUFFER_PATH, "list.txt");

    FILE *list = fopen(BUFFER_PATH, "w");
    fclose(list);
}

void remove_by_name_debug(const char *file_name) {
    char BUFFER_PATH[MAX_BIN_PATH_NAME];
    get_bin_path(BUFFER_PATH, sizeof(BUFFER_PATH));
    strcat(BUFFER_PATH, "trash/");

    DIR *dir = opendir(BUFFER_PATH);
    
    if (!dir) {
        closedir(dir);
        error("Cannot access the bin folder");
    }
    
    if (check_if_file_exist(dir, file_name)) {
        strcat(BUFFER_PATH, file_name);
        
        remove(BUFFER_PATH);
    } else {
        closedir(dir);
        error("File not found");
    }

    printf("File %s removed succesfully\n", file_name);
    remove_log_entry_debug(file_name);
    closedir(dir);
}

void remove_log_entry_debug(const char *file_name) {
    char list_path[MAX_BIN_PATH_NAME];
    get_bin_path(list_path, sizeof(list_path));
    strcat(list_path, "list.txt");

    FILE *list = fopen(list_path, "r");

    if (!list) error("Cannot open list.txt");

    char temp_path[MAX_BIN_PATH_NAME];
    strcpy(temp_path, list_path);
    strcat(temp_path, ".tmp");

    FILE *temp = fopen(temp_path, "w");
    if (!temp) {
        fclose(list);
        error("Cannot create temp file");
    }

    char line[MAX_BUFFER_SIZE_DEBUG];
    while (fgets(line, sizeof(line), list)) {
        char name[MAX_FILE_NAME];
        int i = 0, j = 0, jump = 0;

        for (; line[i]; i++) {
            if (line[i] == '|') jump++;
            else if (jump == 1 && line[i] != ' ') {
                name[j++] = line[i];
            }
        }

        name[j] = '\0';
        if (strcmp(name, file_name) != 0) {
            fputs(line, temp);
        }
    }

    fclose(list);
    fclose(temp);

    if (rename(temp_path, list_path) != 0) {
        error("Cannot replace list.txt");
    }
}

void recover_debug(const char *file_name) {
    char BUFFER_PATH[MAX_BIN_PATH_NAME];
    get_bin_path(BUFFER_PATH, sizeof(BUFFER_PATH));
    
    size_t buffer_l = strlen(BUFFER_PATH);
    strcat(BUFFER_PATH, "trash/");

    DIR *dir = opendir(BUFFER_PATH);
    
    if (!dir) {
        closedir(dir);
        error("Cannot access the bin folder");
    }

    if (check_if_file_exist(dir, file_name)) {
        BUFFER_PATH[buffer_l] = '\0';   
        strcat(BUFFER_PATH, "list.txt");

        FILE *list = fopen(BUFFER_PATH, "r");
        if (!list) error("It was not possible to open list.txt");

        int c, i, j, jump, can_get_path, pos;
        jump = i = j = can_get_path = pos = 0;

        char BUFFER_PATH[MAX_BIN_PATH_NAME];
        char BUFFER_NAME[MAX_FILE_NAME];

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

            if (can_get_path && c == '\n') {
                BUFFER_PATH[j] = '\0';
                break;
            };

            if (c == '\n') {
                i = j = jump = 0;
                memset(BUFFER_NAME, 0 , sizeof(BUFFER_NAME));
            }
        }
        char *ptr = &BUFFER_PATH[0];

        // remove break line
        while (*ptr != '\n') {
            ptr++;
            pos++;
        }

        BUFFER_PATH[pos] = '\0';

        char BIN_PATH[MAX_BIN_PATH_NAME];
        get_bin_path(BIN_PATH, sizeof(BIN_PATH));
        strcat(BIN_PATH, "trash/");

        move_debug(BUFFER_PATH, BIN_PATH, file_name, 0);
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
