#include "shellState.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>


static char currentDir[MAX_PATH_LEN];
static char initialDir[MAX_PATH_LEN];

// Функция для инициализации состояния оболочки
void initShellState() {
    // Получаем текущий рабочий каталог
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("Error retrieving current path");
        // Если не удалось получить путь, устанавливаем корневой каталог
        strcpy(currentDir, "/");
    }
    // Копируем текущий каталог как начальный путь оболочки
    strcpy(initialDir, currentDir);
}


// Функция для получения текущего каталога
const char* getCurrentDir() {
    return currentDir;
}

// Функция для изменения текущего каталога
int changeCurrentDir(const char *path) {
    // Если путь не задан (NULL), возвращаем в начальный каталог
    if (chdir(path == NULL ? initialDir : path) == 0) {
        // Обновляем текущий каталог
        if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
            perror("Error updating current path");
            return -1;                                              // Ошибка при обновлении текущего пути
        }
        return 0;                                                   // Успешное изменение каталога
    }
    perror("Error changing current path");
    return -1;                                                      // Ошибка при смене каталога
}