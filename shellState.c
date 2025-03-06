#include "shellState.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>


static char currentDir[MAX_PATH_LEN];

void initShellState() {
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("Ошибка получения текущего пути");
        strcpy(currentDir, "/");
    }
}

const char* getCurrentDir() {
    return currentDir;
}

int changeCurrentDir(const char *path) {
    if (chdir(path) == 0) {
        if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
            perror("Ошибка обновления текущего пути");
            return -1;
        }
        return 0;
    }
    perror("Ошибка смены текущего пути");
    return -1;
}