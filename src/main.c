#include <stdio.h>

#include "headers/bin.h"

int main(int argc, char* argv[]) {    
    if (argc < 2) error("Insufficient arguments");

    if (strcmp(argv[1], "move") == 0 && argv[2] != NULL) move(argv[2], 0);
    else if (strcmp(argv[1], "list") == 0) list();
    else if (strcmp(argv[1], "match-move") == 0) move_file_match_pattern(argv[1]);
    else if (strcmp(argv[1], "clean") == 0) clean();
    else error("Invalid argv, see available argv:\nmove: Move a specific file by name: trash-rm move [file_name]\nlist: lists all files in the trash: trash-rm list.\nmatch-move: move all files that match a pattern: trash-rm match-move [pattern]");

    return 0;
}
