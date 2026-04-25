#include <stdio.h>
#include "../include/liner.h"
#include "../include/lexer.h"

int main() {
    char code[] = "int main";
    int *len;
    char **splited = lineSplit(code, &len);
    return 0;
}