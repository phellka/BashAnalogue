#include "commandManager.h"
#include "commandsPool.h"
#include "shellState.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>


#define MAX_INPUT 256
#define MAX_ARGS 10

static const char **commands = NULL;

// Генератор команд для автодополнения в readline.
char *commandGenerator(const char *text, int state) {
    static int index, length;
    const char *cmd;

    if (state == 0) {
        index = 0;
        length = strlen(text);
    }

    while ((cmd = commands[index++])) {
        if (strncmp(cmd, text, length) == 0) {
            return strdup(cmd); // Возвращает команду, соответствующую введенному тексту.
        }
    }
    return NULL;
}

// Функция автодополнения команд.
char **commandCompletion(const char *text, int start, int end) {
    (void)end;
    if (start == 0) {
        return rl_completion_matches(text, commandGenerator); // Инициализация автодополнения.
    }

    return NULL;
}

// Парсинг аргументов из ввода пользователя.
static void parseArguments(const char *input, char *argv[MAX_ARGS], int *argc) {
    *argc = 0;
    const char *p = input;
    char buffer[1024];
    int buf_idx = 0;
    int in_arg = 0;

    while (*p) {
        // Обработка специальных символов и пробелов.
        if (*p == '\\' && *(p + 1) == ' ') {
            in_arg = 1;
            buffer[buf_idx++] = ' ';
            p += 2;
        } else if (isspace((unsigned char)*p)) {
            if (in_arg) {
                buffer[buf_idx] = '\0';
                argv[(*argc)++] = strdup(buffer); // Сохранение аргумента.
                buf_idx = 0;
                in_arg = 0;
            }
            p++;
        } else {
            in_arg = 1;
            buffer[buf_idx++] = *p++; // Собираем символы в аргумент.
        }
    }

    if (in_arg) {
        buffer[buf_idx] = '\0';
        argv[(*argc)++] = strdup(buffer);
    }

    argv[*argc] = NULL; // Завершаем список аргументов.
}

// Обработка ввода пользователя.
static int processInput(char *input) {
    input[strcspn(input, "\n")] = '\0'; // Убираем символ новой строки.
    if (strcmp(input, "") == 0 || strcmp(input, "\n") == 0) {
        return 1; // Пустой ввод.
    }

    // Проверка на команды exit и --help.
    if (strcmp(input, "exit") == 0) {
        return 0; // Выход из программы.
    }
    if (strcmp(input, "--help") == 0) {
        printCommandsList(); // Печать списка команд.
        return 1;
    }
    char *argv[MAX_ARGS];
    int argc = 0;
    parseArguments(input, argv, &argc); // Парсим аргументы.


    Command* command = findCommand(argv[0]);
    if (command) {
        command->exec(argc, argv); // Выполнение команды.
    }
    else {
        printf("Command not found, use --help for a list of commands\n");
    }

    for (int i = 0; i < argc; i++) {
        free(argv[i]); // Освобождение памяти.
    }
    return 1;
}

// Инициализация менеджера команд.
void initCommandManager() {
    initCommandsPool();                                   // Инициализация пула команд.
    commands = getCommandsName();                         // Получение имен команд.
    rl_attempted_completion_function = commandCompletion; // Настройка автодополнения.
    initShellState();                                     // Инициализация состояния оболочки.
}

// Получение строки подсказки для пользователя.
char *getPrompt() {
    static char prompt[1024];
    snprintf(prompt, sizeof(prompt), "\033[1;34m>%s: \033[0m", getCurrentDir()); // Формирование подсказки.
    return prompt;
}

// Основной цикл оболочки.
void runShell() {
    char *input;
    while (1) {
        input = readline(getPrompt()); // Чтение ввода с консоли.
        if (!input) {
            continue;
        }
        if (*input) {
            add_history(input); // Добавление в историю.
        }
        if (processInput(input) == 0) {
            free(input); // Выход из программы.
            break;
        }
        free(input);
    }
}