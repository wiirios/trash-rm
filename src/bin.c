#include "headers/bin.h"

void move(const char *dest, const char *src, const char *file_name, unsigned char flag) {
    DIR *dest_ = opendir(dest);
    DIR *src_ = opendir(src);

    if (!dest_) error("Cannot open dest folder");

    if (!src_) error("Cannot open src folder");

    if (check_if_file_exist(src_, file_name)) goto found;

    closedir(dest_);
    closedir(src_);
    error("File not found");

    found:
    char SRC_PATH[MAX_PATH_SIZE];
    char DEST_PATH[MAX_PATH_SIZE];
    
    int written = snprintf(SRC_PATH, sizeof(SRC_PATH), "%s%s%s", src, src[strlen(src) - 1] == '/' ? "" : "/", file_name);
    if (written < 0 || (size_t)written >= sizeof(SRC_PATH)) error("Path too long");

    written = snprintf(DEST_PATH, sizeof(DEST_PATH), "%s%s%s", dest, dest[strlen(dest) - 1] == '/' ? "" : "/", file_name);
    if (written < 0 || (size_t)written >= sizeof(DEST_PATH)) error("Path too long");

    size_t size_file;
    if (get_size(SRC_PATH, &size_file) != 0) error("Cannot get file size");

    char q;
    if (check_if_file_exist(dest_, file_name) == 1) {
        while (1) {
            printf("File %s already exists, do you want to overwrite it?\nYes [Y]\nNo [N]\n", file_name);
            scanf("%c", &q);

            if (q == 'Y' || q == 'y' || q == 'N' || q == 'n') break;
        }
        
        if (q == 'N' || q == 'n') error("The file was not overwritten");
    }

    if ((rename(SRC_PATH, DEST_PATH)) == 0) printf("File %s move succesfully\n", file_name);
    else {
        error("Error moving file");
    }
    ;

    if (flag) {
        char TRASH_FOLDER_ROOT[MAX_PATH_SIZE];
        
        strcpy(TRASH_FOLDER_ROOT, dest);
        TRASH_FOLDER_ROOT[strlen(TRASH_FOLDER_ROOT) - 6] = '\0';
        strcat(TRASH_FOLDER_ROOT, "list.txt");

        FILE *list_ = fopen(TRASH_FOLDER_ROOT, "a");
        
        if (!list_) error("It was not possible to open list.txt");
        
        time_t today = time(NULL);
        set_format(TIMESTAMP_FORMAT);

        fprintf(list_, "%s | %s | %s\n", get_date(&today), file_name, src);
        fclose(list_);
    }

    closedir(dest_);
    closedir(src_);
}

int list() {
    char TRASH_RM_FOLDER_LIST[MAX_PATH_SIZE];
    get_origin_path(TRASH_RM_FOLDER_LIST, sizeof(TRASH_RM_FOLDER_LIST));
    
    strcat(TRASH_RM_FOLDER_LIST, "list.txt");

    FILE *list_ = fopen(TRASH_RM_FOLDER_LIST, "r");
    if (!list_) error("It was not possible to open list.txt");

    char line[MAX_BUFFER_SIZE];

    while (fgets(line, sizeof(line), list_)) printf("%s", line);

    fclose(list_);
    return 1;
}

void move_file_match_pattern(const char *pattern) {
    char TRASH_RM_FOLDER[MAX_PATH_SIZE];
    get_origin_path(TRASH_RM_FOLDER, sizeof(TRASH_RM_FOLDER));
    strcat(TRASH_RM_FOLDER, "trash/");
    
    char USER_PATH[MAX_PATH_SIZE];
    GetCurrentDir(USER_PATH, MAX_PATH_SIZE);
    DIR *current_dir = opendir(USER_PATH);

    regex_t regex;
    int status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status) {
        regfree(&regex);
        error("Could not compile the regex");
    }

    if (!current_dir) error("Cannot access the bin folder");
    
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
            move(TRASH_RM_FOLDER, USER_PATH, d->d_name, 1);
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
    int result = list();

    if (!result) return;
    
    char BUFFER_PATH[MAX_PATH_SIZE];
    get_origin_path(BUFFER_PATH, sizeof(BUFFER_PATH));

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

    char BUFFER_FILE_PATH[MAX_PATH_SIZE];
    while ((d = readdir(dir)) != NULL) if (d->d_type == 8) {
        int written = snprintf(BUFFER_FILE_PATH, sizeof(BUFFER_FILE_PATH), "%s%s", BUFFER_PATH, d->d_name);
        
        if (written < 0 || (size_t)written >= sizeof(BUFFER_FILE_PATH)) error("Path too long");

        remove(BUFFER_FILE_PATH);
        memset(BUFFER_FILE_PATH, 0, sizeof(BUFFER_FILE_PATH));
    }

    
    BUFFER_PATH[strlen(BUFFER_PATH) - 6] = '\0';
    strcat(BUFFER_PATH, "list.txt");

    FILE *list = fopen(BUFFER_PATH, "w");
    fclose(list);
}

int get_size(const char *file_name, size_t *out_size) {
    struct stat st;

    if ((stat(file_name, &st)) != 0) return -1;

    if ((size_t) st.st_size > __SIZE_MAX__ || st.st_size < 0) return -1;

    *out_size = (size_t) st.st_size;

    return 0;
}

void remove_by_name_from_bin(const char *file_name) {
    char TRASH_FOLDER_ROOT[MAX_PATH_SIZE];
    get_origin_path(TRASH_FOLDER_ROOT, sizeof(TRASH_FOLDER_ROOT));
    strcat(TRASH_FOLDER_ROOT, "trash/");

    DIR *dir = opendir(TRASH_FOLDER_ROOT);
    
    if (!dir) error("Cannot access the bin folder");
    
    if (check_if_file_exist(dir, file_name)) {
        strcat(TRASH_FOLDER_ROOT, file_name);
        
        remove(TRASH_FOLDER_ROOT);
    } else {
        closedir(dir);
        error("File not found");
    }

    printf("File %s removed succesfully\n", file_name);
    closedir(dir);
}


void recover(const char *file_name) {
    char TRASH_FOLDER[MAX_PATH_SIZE];
    get_origin_path(TRASH_FOLDER, sizeof(TRASH_FOLDER));
    
    size_t buffer_l = strlen(TRASH_FOLDER);
    strcat(TRASH_FOLDER, "trash/");

    DIR *dir = opendir(TRASH_FOLDER);
    
    if (!dir) {
        closedir(dir);
        error("Cannot access the bin folder");
    }

    if (check_if_file_exist(dir, file_name)) {
        TRASH_FOLDER[buffer_l] = '\0';   
        strcat(TRASH_FOLDER, "list.txt");

        FILE *list = fopen(TRASH_FOLDER, "r");
        if (!list) error("It was not possible to open list.txt");

        int c, i, j, jump, can_get_path, pos;
        jump = i = j = can_get_path = pos = 0;

        char BUFFER_PATH[MAX_PATH_SIZE];
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

        char BIN_PATH[MAX_PATH_SIZE];
        get_origin_path(BIN_PATH, sizeof(BIN_PATH));
        strcat(BIN_PATH, "trash/");

        move(BUFFER_PATH, BIN_PATH, file_name, 0);
        closedir(dir);
        fclose(list);
    } else {
        closedir(dir);
        error("File not found");
    }

}
