#include "CdCommand.h"
#include <stdio.h>
#include <string.h>
#include "../shellState.h"

// Функция помощи для команды cd
static void help() {
    printf("cd: cd DIR\n      Change the current directory to DIR.  The default DIR is the value of the HOME shell variable.\n");
}

// Основная функция выполнения команды cd
static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    if (argc > 1) {
        // Меняем директорию на указанную
        changeCurrentDir(argv[1]);
    }
    else {
        // Если параметр не передан, меняем на домашнюю директорию
        changeCurrentDir(NULL);    
    }
}

// Структура для команды cd
Command cmd_cd = {
    .name = "cd",
    .exec = exec,
    .help = help
};