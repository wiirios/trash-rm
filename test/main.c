#include "bin.h"

#include <stdio.h>

#define DEBUG 1

/*
    TEST CASES
*/

#ifdef DEBUG
int main(int argc, char* argv[]) {
    // move_file_match_pattern(argv[1], argv[2]);

    if (argc < 2) error("Insufficient arguments");
    
    char BUFFER_PATH[MAX_BIN_PATH_NAME];
    get_bin_path(BUFFER_PATH, sizeof(BUFFER_PATH));

    create_bin(BUFFER_PATH);
    strcat(BUFFER_PATH, "/trash");
    create_bin(BUFFER_PATH);
    strcat(BUFFER_PATH, "/list.txt");

    FILE *f_ = fopen(BUFFER_PATH, "w");
    
    if (!f_) {
        fclose(f_);
        error("Cannot create list.txt");
    }

    fclose(f_);

    if (strcmp(argv[1], "move") == 0 && argv[2] != NULL) move_debug(argv[2], 0);
    else if (strcmp(argv[1], "list") == 0) list_debug();
    else if (strcmp(argv[1], "match-move") == 0) move_file_match_pattern(argv[1]);
    else if (strcmp(argv[1], "clean") == 0) clean();
    else if (strcmp(argv[1], "remove") == 0) remove_by_name_debug(argv[2]);
    else if (strcmp(argv[1], "recover") == 0) recover_debug(argv[2]);
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
