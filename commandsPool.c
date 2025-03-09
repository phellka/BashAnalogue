#include "commandsPool.h"
#include <stdio.h>
#include <string.h>
#include "commands/PwdCommand.h"
#include "commands/LsCommand.h"
#include "commands/CdCommand.h"
#include "commands/GrepCommand.h"
#include "commands/PingCommand.h"
#include "commands/CpCommand.h"


#define maxCommandsLen 10

static Command *commands[maxCommandsLen];
static const char *commandsNames[maxCommandsLen];
static int commandCount = 0;

// Регистрация новой команды в пуле команд.
void registerCommand(Command *command) {
    if (command) {
        if (commandCount < maxCommandsLen) {
            commandsNames[commandCount] = command->name;
            commands[commandCount++] = command; // Добавляем команду в список.
        }
        else {
            fprintf(stderr, "Command limit exceeded\n"); // Превышен лимит команд.
        }
    }
}

// Инициализация пула команд.
void initCommandsPool() {
    registerCommand(&cmd_pwd);
    registerCommand(&cmd_ls);
    registerCommand(&cmd_cd);
    registerCommand(&cmd_grep);
    registerCommand(&cmd_ping);
    registerCommand(&cmd_cp);
}

// Печать списка доступных команд.
void printCommandsList() {
    printf("Available commands:\n");
    for (int i = 0; i < commandCount; ++i) {
        printf(" - %s\n", commands[i]->name);
    }
}

// Поиск команды по имени.
Command* findCommand(const char* name) {
    for (int i = 0; i < commandCount; ++i) {
        if (strcmp(commands[i]->name, name) == 0) {
            return commands[i];
        }
    }
    return NULL; // Команда не найдена.
}

// Получение массива имен команд.
const char **getCommandsName() {
    return commandsNames;
}