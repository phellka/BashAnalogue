#include "GrepCommand.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Функция помощи для команды grep
static void help() {
    printf("grep: grep PATTERN FILE\n      Searches for PATTERN in FILE.\n");
}

// Основная функция выполнения команды grep
static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    if (argc < 3) {
        printf("Invalid parameters, use --help for more information\n");
        return;
    }

    // Открытие файла для поиска по нему
    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Поиск паттерна в каждой строке файла
    char line[BUFFER_SIZE];
    int pattern_len = strlen(argv[1]);
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, argv[1])) {
            char *pos = strstr(line, argv[1]);
            if (pos) {
                fwrite(line, 1, pos - line, stdout);
                printf("\033[1;33m%.*s\033[0m", pattern_len, pos);
                printf("%s", pos + pattern_len);
                if (line[strlen(line) - 1] != '\n') {
                    printf("\n");
                }
            }
        }
    }

    fclose(file);
}


// Структура для команды grep
Command cmd_grep = {
    .name = "grep",
    .exec = exec,
    .help = help
};