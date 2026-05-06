#include "headers/bin.h"

void move(const char *dest, const char *src, const char *file_name, unsigned char flag) {
    DIR *dest_ = opendir(dest);
    DIR *src_ = opendir(src);

    if (!dest_ || !src_) error("Cannot open dest folder or src folder");

    if (check_if_file_exist(src_, file_name)) goto found;

    closedir(dest_);
    closedir(src_);
    error("File not found");

    found:
    size_t size_file;
    if (get_size(file_name, &size_file) != 0) error("Cannot get file size");

    char *BUFFER = malloc(size_file + 1);
    if (!BUFFER) error("Error allocating memory for Buffer");

    char q;
    if (check_if_file_exist(dest_, file_name) == 1) {
        while (1) {
            printf("File %s already exists, do you want to overwrite it?\nYes [Y]\nNo [N]\n", file_name);
            scanf("%c", &q);

            if (q == 'Y' || q == 'y' || q == 'N' || q == 'n') break;
        }
        
        if (q == 'N' || q == 'n') error("The file was not overwritten");
    }

    snprintf(BUFFER, size_file, "%s%s", dest, file_name);
    if ((rename(file_name, BUFFER)) == 0) printf("File %s move succesfully\n", file_name);
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
    free(BUFFER);
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
