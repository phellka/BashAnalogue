#include "shellState.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>


static char currentDir[MAX_PATH_LEN];
static char initialDir[MAX_PATH_LEN];

void initShellState() {
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("Error retrieving current path");
        strcpy(currentDir, "/");
    }
    strcpy(initialDir, currentDir);
}

const char* getCurrentDir() {
    return currentDir;
}

int changeCurrentDir(const char *path) {
    if (chdir(path == NULL ? initialDir : path) == 0) {
        if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
            perror("Error updating current path");
            return -1;
        }
        return 0;
    }
    perror("Error changing current path");
    return -1;
}