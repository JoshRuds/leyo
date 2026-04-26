#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/errors.h"

static Error errors[100];
static int error_count = 0;

bool isErr = false;

void raise(const char *msg, int line, int col) {
    isErr = true;

    if (error_count >= 100) return;

    strncpy(errors[error_count].message,
            msg,
            sizeof(errors[error_count].message) - 1);

    errors[error_count].message[sizeof(errors[error_count].message) - 1] = '\0';

    errors[error_count].line = line;
    errors[error_count].column = col;

    error_count++;
}

void callAllErr() {
    for (int i = 0; i < error_count; i++) {
        Error currErr = errors[i];
        printf("Error at position %d:%d: %s\n\n",
               currErr.line,
               currErr.column,
               currErr.message);
    }
    quick_exit(1);
}