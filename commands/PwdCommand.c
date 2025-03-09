#include "PwdCommand.h"
#include <stdio.h>
#include <string.h>
#include "../shellState.h"

// Функция помощи для команды pwd
static void help() {
    printf("pwd: pwd\n      Print the name of the current working directory.\n");
}

// Основная функция выполнения команды pwd
static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    printf("%s\n", getCurrentDir());
}

// Структура для команды pwd
Command cmd_pwd = {
    .name = "pwd",
    .exec = exec,
    .help = help
};