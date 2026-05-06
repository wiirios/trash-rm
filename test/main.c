#include "bin.h"

#include <stdio.h>

#define DEBUG 1

/*
    TEST CASES
*/

#ifdef DEBUG
int main(int argc, char* argv[]) {
    if (argc < 2) error("Insufficient arguments");
    
    char BIN_PATH[MAX_BIN_PATH_NAME], LIST_BIN_PATH[MAX_BIN_PATH_NAME];
    get_bin_path(BIN_PATH, sizeof(BIN_PATH));
    get_bin_path(LIST_BIN_PATH, sizeof(LIST_BIN_PATH));
    
    create_bin(BIN_PATH);
    DIR *dir = opendir(BIN_PATH);
    
    if (!check_if_file_exist(dir, "list.txt")) {
        strcat(LIST_BIN_PATH, "list.txt");
        FILE *f_ = fopen(LIST_BIN_PATH, "w");
        
        if (!f_) error("Cannot create list.txt");
        
        fclose(f_);
    }
    closedir(dir);

    strcat(BIN_PATH, "trash/");
    create_bin(BIN_PATH);
    
    char USER_PATH[MAX_FILE_NAME];
    GetCurrentDir(USER_PATH, FILENAME_MAX);

    if (strcmp(argv[1], "move") == 0 && argv[2] != NULL) move_debug(BIN_PATH, USER_PATH, argv[2], 1);
    else if (strcmp(argv[1], "list") == 0) list_debug();
    else if (strcmp(argv[1], "match-move") == 0) move_file_match_pattern(argv[1]);
    else if (strcmp(argv[1], "clean") == 0) clean();
    else if (strcmp(argv[1], "remove") == 0 && argv[2] != NULL) remove_by_name_debug(argv[2]);
    else if (strcmp(argv[1], "recover") == 0 && argv[2] != NULL) recover_debug(argv[2]);
    else error("Invalid argv, see available argv:\nmove: Move a specific file by name: trash-rm move [file_name]\nlist: lists all files in the trash: trash-rm list.\nmatch-move: move all files that match a pattern: trash-rm match-move [pattern]");

    return 0;
}

#else
int main(int argc, char* argv[]) {
    // GET TIMESTAMP
    // if (set_format("DDMMYYYY")) printf("%s\n", get_date(&today));

    // FILE TO MOVE
    // move_debug(argv[1]);

    list_debug(PATH_BIN);

    return 0;
}

#endif
