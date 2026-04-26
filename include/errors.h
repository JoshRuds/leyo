#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

typedef struct {
    char message[256];
    int line;
    int column;
} Error;

void raise(const char *msg, int line, int col);
void callAllErr();

extern bool isErr;

#endif