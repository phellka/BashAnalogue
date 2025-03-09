#include "LsCommand.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../shellState.h"

// Функция для сортировки имен файлов
static int compareNames(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

// Функция помощи для команды ls
static void help() {
    printf("ls: ls [FILE]\n      Displays information about FILE (by default, the current directory).\n");
}

// Функция для построения полного пути из базового и относительного пути
static char* buildPath(const char *base, const char *subpath) {
    size_t len = strlen(base) + strlen(subpath) + 2;
    char *fullPath = malloc(len);
    if (!fullPath) {
        perror("Memory allocation error");
        return NULL;
    }
    snprintf(fullPath, len, "%s/%s", base, subpath);
    return fullPath;
}

// Функция для отображения содержимого директории
static void listDirectory(const char *dirPath) {
    struct dirent *entry;
    DIR *dir = opendir(dirPath);
    
    if (dir == NULL) {
        perror("Failed to open directory");
        return;
    }

    char **names = NULL;
    size_t count = 0;

    // Чтение содержимого директории
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        names = realloc(names, (count + 1) * sizeof(char *));
        if (!names) {
            perror("Memory allocation error");
            closedir(dir);
            return;
        }
        names[count] = strdup(entry->d_name);
        if (!names[count]) {
            perror("Memory allocation error");
            closedir(dir);
            return;
        }
        count++;
    }

    closedir(dir);

    // Сортировка и вывод файлов
    qsort(names, count, sizeof(char *), compareNames);
    for (size_t i = 0; i < count; i++) {
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, names[i]);
        struct stat pathStat;
        if (stat(fullPath, &pathStat) == 0 && S_ISDIR(pathStat.st_mode)) {
            printf("\033[1;34m%s/\033[0m\n", names[i]); // Директории выделяем цветом
        } else {
            printf("%s\n", names[i]);
        }
        free(names[i]);
    }

    free(names);
}

// Основная функция выполнения команды ls
static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    const char *currentDir = getCurrentDir();
    char *dirPath;
    if (argc > 1) {
        if (argv[1][0] == '/') {
            dirPath = strdup(argv[1]);
        }
        else {
            dirPath = buildPath(currentDir, argv[1]);
        }
    }
    else {
        dirPath = strdup(currentDir);
    }
    
    if (!dirPath) {
        perror("Memory allocation error");
        return;
    }

    listDirectory(dirPath);
    free(dirPath);
}

// Структура для команды ls
Command cmd_ls = {
    .name = "ls",
    .exec = exec,
    .help = help
};