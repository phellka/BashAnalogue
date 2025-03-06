#pragma once

#include "Command.h"


void initCommandsPool();
Command* findCommand(const char* name);
void printCommandsList();
