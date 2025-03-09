# Консольный интерпретатор командной строки

## Описание
Данный проект представляет собой консольный интерпретатор командной строки, реализующий базовые команды оболочки Linux: `ls`, `cd`, `pwd`, `grep`, `ping`, `cp`. Также поддерживается автодополнение команд по нажатию клавиши `Tab`.

**Примечание:** Проект использует веб-сокеты, которые требуют запуска с `sudo`.
## Функциональность
- **ls** [path] - выводит список файлов в текущей или указанной директории.
- **cd** path - изменяет текущую директорию.
- **pwd** - выводит текущую директорию.
- **grep** pattern file - ищет заданный паттерн в указанном файле.
- **ping** hostname/ip - отправляет 4 ICMP-запроса и выводит статистику.
- **cp** source destination - копирует файл в указанное место.
- **Автодополнение команд** - при вводе начала команды и нажатии `Tab` производится автодополнение.

## Требования
- **ОС:** Ubuntu 22.04+
- **Компилятор:** GCC
- **Система сборки:** CMake
- **Библиотеки:** Readline

## Сборка и установка
```sh
sudo apt update

# Установка инструментов сборки
sudo apt install -y cmake gcc make

# Установка библиотеки readline
sudo apt install -y libreadline-dev

mkdir build && cd build
cmake ..
make
```

## Запуск
```sh
sudo ./main
```

## Структура проекта
- `main.c` - точка входа
- `commandManager.c/h` - управление командами
- `commandsPool.c/h` - регистрация команд
- `shellState.c/h` - управление состоянием оболочки
- `commands/` - каталог с реализациями команд
