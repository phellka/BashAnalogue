#include "CpCommand.h"
#include <stdio.h>
#include <string.h>

// Функция помощи для команды cp
static void help() {
    printf("cp: cp SOURCE DESTINATION\n      Copies SOURCE to DESTINATION\n");
}

// Основная функция выполнения команды cp
static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    if (argc < 3) {
        printf("Invalid parameters, use --help for more information\n");
        return;
    }

    // Открытие исходного файла для чтения
    FILE *fileInput = fopen(argv[1], "r");
    if (fileInput == NULL) {
        perror("Error opening file");
        return;
    }

    // Открытие файла назначения для записи
    FILE *fileOutput = fopen(argv[2], "w");
    if (fileOutput == NULL) {
        perror("Error opening file");
        return;
    }

    // Копирование содержимого файла
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

// Структура для команды cp
Command cmd_cp = {
    .name = "cp",
    .exec = exec,
    .help = help
};