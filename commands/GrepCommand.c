#include "GrepCommand.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

static void help() {
    printf("grep: grep ШАБЛОН [ФАЙЛ]…\n     Поиск ШАБЛОНА в ФАЙЛЕ.\n");
}

static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    if (argc < 3) {
        printf("Неправильные параметры, --help для справки\n");
        return;
    }
    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }
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

Command cmd_grep = {
    .name = "grep",
    .exec = exec,
    .help = help
};