#include <stdlib.h>

int lineCounter (char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == ';') {
            count++;
        };
    };
    return count;
}

char **lineSplit (char *code, int *len) {
    int count = lineCounter(code);
    *len = count;
    if (count == 0) count = 1;
    char **lines = malloc(sizeof(char*) * count);

    int index = 0;
    lines[index] = code;

    for (int i = 0; code[i]; i++) {
        if (code[i] == ';') {
            code[i] = '\0';          
            lines[++index] = &code[i+1]; 
        }
    }
}