#include "CpCommand.h"
#include <stdio.h>
#include <string.h>


static void help() {
    printf("cp: cp SOURCE DESTINATION\n      Copies SOURCE to DESTINATION\n");
}

static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    if (argc < 3) {
        printf("Invalid parameters, use --help for more information\n");
        return;
    }

    FILE *fileInput = fopen(argv[1], "r");
    if (fileInput == NULL) {
        perror("Error opening file");
        return;
    }

    FILE *fileOutput = fopen(argv[2], "w");
    if (fileOutput == NULL) {
        perror("Error opening file");
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