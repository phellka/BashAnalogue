#pragma once

typedef struct
{
    const char *name;
    void (*exec)(int argc, char **argv);
    void (*help)();
} Command;
