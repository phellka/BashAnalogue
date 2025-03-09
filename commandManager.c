#include "commandManager.h"
#include "commandsPool.h"
#include "shellState.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_ARGS 10

static void parseArguments(const char *input, char *argv[MAX_ARGS], int *argc) {
    *argc = 0;
    const char *p = input;
    char buffer[1024];
    int buf_idx = 0;
    int in_arg = 0;

    while (*p) {
        if (*p == '\\' && *(p + 1) == ' ') {
            in_arg = 1;
            buffer[buf_idx++] = ' ';
            p += 2;
        } else if (isspace((unsigned char)*p)) {
            if (in_arg) {
                buffer[buf_idx] = '\0';
                argv[(*argc)++] = strdup(buffer); 
                buf_idx = 0;
                in_arg = 0;
            }
            p++;
        } else {
            in_arg = 1;
            buffer[buf_idx++] = *p++;
        }
    }

    if (in_arg) {
        buffer[buf_idx] = '\0';
        argv[(*argc)++] = strdup(buffer);
    }

    argv[*argc] = NULL;
}

static int processInput(char *input) {
    input[strcspn(input, "\n")] = '\0';
    if (strcmp(input, "exit") == 0) {
        return 0;
    }
    if (strcmp(input, "--help") == 0) {
        printCommandsList();
        return 1;
    }
    char *argv[MAX_ARGS];
    int argc = 0;
    parseArguments(input, argv, &argc);


    Command* command = findCommand(argv[0]);
    if (command) {
        command->exec(argc, argv);
    }
    else {
        printf("Command not found, use --help for a list of commands\n");
    }

    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    return 1;
}

void initCommandManager() {
    initCommandsPool();
    initShellState();
}

void printPromt() {
    printf("\033[1;34m>%s: \033[0m", getCurrentDir());
}

void runShelll() {
    char input[MAX_INPUT];
    while (1) {
        printPromt();
        if (!fgets(input, MAX_INPUT, stdin)) {
            break;
        }
        if (processInput(input) == 0) {
            break;
        }
    }
}