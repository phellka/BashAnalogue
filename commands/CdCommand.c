#include "CdCommand.h"
#include <stdio.h>
#include <string.h>
#include "../shellState.h"

static void help() {
    printf("cd: cd [каталог]\n      Change the current directory to DIR.  The default DIR is the value of the HOME shell variable.\n");
}

static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    if (argc > 1) {
        changeCurrentDir(argv[1]);
    }
    else {
        changeCurrentDir(NULL);    
    }
}


Command cmd_cd = {
    .name = "cd",
    .exec = exec,
    .help = help
};