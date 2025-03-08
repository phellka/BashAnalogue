#include "CpCommand.h"
#include <stdio.h>
#include <string.h>


static void help() {
    printf("cp: cp  [ИСТОЧНИК] [НАЗНАЧЕНИЕ]\n       Копирует ИСТОЧНИК в НАЗНАЧЕНИЕ\n");
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

    FILE *fileInput = fopen(argv[1], "r");
    if (fileInput == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    FILE *fileOutput = fopen(argv[2], "w");
    if (fileOutput == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    char buffer;
    buffer = fgetc(fileInput);
    while (buffer != EOF)
    {
        fputc(buffer, fileOutput);
        buffer = fgetc(fileInput);
    }

    printf("Copying completed\n");
    fclose(fileInput);
    fclose(fileOutput);
}


Command cmd_cp = {
    .name = "cp",
    .exec = exec,
    .help = help
};