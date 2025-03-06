#include "LsCommand.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include "../shellState.h"

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

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (entry->d_type == DT_DIR) {
            printf("\033[1;34m%s/\033[0m\n", entry->d_name);
        }
        else {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
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