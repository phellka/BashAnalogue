#include "LsCommand.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../shellState.h"

#define BLUE "\033[1;34m"
#define RESET "\033[0m"

static int compareNames(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

static void help() {
    printf("ls: ls [ФАЙЛ]…\n Выдаёт информацию о ФАЙЛАХ (по умолчанию о текущем каталоге).\n");
}

static char* buildPath(const char *base, const char *subpath) {
    size_t len = strlen(base) + strlen(subpath) + 2;
    char *fullPath = malloc(len);
    if (!fullPath) {
        perror("Ошибка выделения памяти");
        return NULL;
    }
    snprintf(fullPath, len, "%s/%s", base, subpath);
    return fullPath;
}

static void listDirectory(const char *dirPath) {
    struct dirent *entry;
    DIR *dir = opendir(dirPath);
    
    if (dir == NULL) {
        perror("Не удалось открыть директорию");
        return;
    }

    char **names = NULL;
    size_t count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        names = realloc(names, (count + 1) * sizeof(char *));
        if (!names) {
            perror("Ошибка выделения памяти");
            closedir(dir);
            return;
        }
        names[count] = strdup(entry->d_name);
        if (!names[count]) {
            perror("Ошибка выделения памяти");
            closedir(dir);
            return;
        }
        count++;
    }

    closedir(dir);

    qsort(names, count, sizeof(char *), compareNames);

    for (size_t i = 0; i < count; i++) {
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, names[i]);
        struct stat pathStat;
        if (stat(fullPath, &pathStat) == 0 && S_ISDIR(pathStat.st_mode)) {
            printf("\033[1;34m%s/\033[0m\n", names[i]);
        } else {
            printf("%s\n", names[i]);
        }
        free(names[i]);
    }

    free(names);
}

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
        perror("Ошибка выделения памяти");
        return;
    }

    listDirectory(dirPath);
    free(dirPath);
}

Command cmd_ls = {
    .name = "ls",
    .exec = exec,
    .help = help
};