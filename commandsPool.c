#include "commandsPool.h"
#include <stdio.h>
#include <string.h>
#include "commands/PwdCommand.h"


#define maxCommandsLen 10

static Command *commands[maxCommandsLen];
static int commandCount = 0;

void registerCommand(Command *command) {
    if (command) {
        if (commandCount < maxCommandsLen) {
            commands[commandCount++] = command;
        }
        else {
            fprintf(stderr, "Превышен лимит команд\n");
        }
    }
}

void initCommandsPool() {
    registerCommand(&cmd_pwd);
}

void printCommandsList() {
    printf("Доступные команды:\n");
    for (int i = 0; i < commandCount; ++i) {
        printf(" - %s\n", commands[i]->name);
    }
}

Command* findCommand(const char* name) {
    for (int i = 0; i < commandCount; ++i) {
        if (strcmp(commands[i]->name, name) == 0) {
            return commands[i];
        }
    }
    return NULL;
}