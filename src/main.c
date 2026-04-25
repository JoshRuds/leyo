#include <stdio.h>
#include "../include/liner.h"
#include "../include/lexer.h"

void printTokenStream(TokenStream *ts) {
    for (int i = 0; i < ts->count; i++) {
        Token t = ts->stream[i];

        printf("Token %d:\n", i);
        printf("  Type   : %s\n", tokenTypeToString(t.type));
        printf("  Value  : %s\n", t.value);
        printf("  Line   : %d\n", t.line);
        printf("  Column : %d\n", t.collumn);
        printf("\n");
    }
}

int main() {
    char code[] = "int main";
    int *len;
    char **splited = lineSplit(code, &len);
    for (int i = 0; i < len; i++) {
        printTokenStream(tokenise(splited[i], i+1));
    };
    return 0;
}