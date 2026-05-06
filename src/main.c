#include <stdio.h>

#include "headers/bin.h"

int main(int argc, char* argv[]) {    
    if (argc < 2) error("Insufficient arguments");

    char TRASH_RM_FOLDER[MAX_PATH_SIZE];
    get_origin_path(TRASH_RM_FOLDER, sizeof(TRASH_RM_FOLDER));
    
    // create trash-rm/
    create_bin(TRASH_RM_FOLDER);
    DIR *trash_rm_path = opendir(TRASH_RM_FOLDER);

    if (!trash_rm_path) error("Cannot open or create the trash-rm root folder");

    if (!check_if_file_exist(trash_rm_path, "list.txt")) {
        char TRASH_RM_FOLDER_LIST[MAX_PATH_SIZE];
        get_origin_path(TRASH_RM_FOLDER_LIST, sizeof(TRASH_RM_FOLDER_LIST));
    
        strcat(TRASH_RM_FOLDER_LIST, "list.txt");
        FILE *list_ = fopen(TRASH_RM_FOLDER_LIST, "w");
        
        if (!list_) error("Cannot create list.txt");
        
        fclose(list_);
    }
    closedir(trash_rm_path);

    // create trash-rm/trash/
    strcat(TRASH_RM_FOLDER, "trash/");
    create_bin(TRASH_RM_FOLDER);

    char USER_PATH[MAX_PATH_SIZE];
    GetCurrentDir(USER_PATH, MAX_PATH_SIZE);

    if (strcmp(argv[1], "move") == 0 && argv[2] != NULL) {
        if (strlen(argv[2]) > MAX_FILE_NAME) error("The file name is too long");
        
        move(TRASH_RM_FOLDER, USER_PATH, argv[2], 1);
    }
    else if (strcmp(argv[1], "list") == 0) list();
    else if (strcmp(argv[1], "match-move") == 0) move_file_match_pattern(argv[2]);
    else if (strcmp(argv[1], "clean") == 0) clean();
    else if (strcmp(argv[1], "remove") == 0 && argv[2] != NULL) remove_by_name_from_bin(argv[2]);
    else error("Invalid argv, see available argv:\nmove: Move a specific file by name: trash-rm move [file_name]\nlist: lists all files in the trash: trash-rm list.\nmatch-move: move all files that match a pattern: trash-rm match-move [pattern]");

    return 0;
}
