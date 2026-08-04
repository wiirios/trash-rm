#include "bin.h"

#if defined(__linux__) || defined(__FreeBSD__)
#include <getopt.h>
#endif

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

    static struct option long_options[] = {
        {"move", required_argument, 0, 'm'},
        {"list", no_argument, 0, 'l'},
        {"match", required_argument, 0, 'e'},
        {"clean", no_argument, 0, 'c'},
        {"remove", required_argument, 0, 'd'},
        {"recover", required_argument, 0, 'r'},
        {"insecure", required_argument, 0, 'i'},
        {0, 0, 0, 0}
    };

    int opt, long_ind;
    opt = long_ind = 0;

    opt = getopt_long(argc, argv, "m:le:cd:r:i:", long_options, &long_ind);

    switch (opt)
    {
    case 'm':
        move_debug(BIN_PATH, USER_PATH, optarg, 1);
        break;
    case 'l':
        list_debug();
        break;
    case 'e':
        move_file_match_pattern(optarg);
        break;
    case 'c':
        clean();
        break;
    case 'd':
        remove_by_name_debug(optarg);
        break;
    case 'r':
        recover_debug(optarg);
        break;
    case 'i':
		set_insecure_path_debug(optarg);
		break;
    default:
    error("Invalid argv, see available argv:\n"
        "move: Move a specific file by name: trash-rm move [file_name]\n"
        "list: lists all files in the trash: trash-rm list.\n"
        "match-move: move all files that match a pattern: trash-rm match-move [pattern]");
    }

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
