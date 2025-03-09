#include <stdio.h>
#include "commandManager.h"



int main() {
    initCommandManager();  // Инициализация менеджера команд.
    runShell();            // Запуск оболочки командной строки.
    return 0;
}