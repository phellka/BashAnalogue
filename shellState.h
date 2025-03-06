#pragma once

#define MAX_PATH_LEN 1024



void initShellState();
const char* getCurrentDir();
int changeCurrentDir(const char *path);