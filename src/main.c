#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"
#include "../include/errors.h"

void printTokenStream(TokenStream ts) {
    for (int i = 0; i < ts.count; i++) {
        Token t = ts.stream[i];

        printf("Token %d:\n", i);
        printf("  Type   : %d\n", t.type);
        printf("  Value  : %s\n", t.value);
        printf("  Line   : %d\n", t.line);
        printf("  Column : %d\n", t.collumn);
        printf("\n");
    }
}

int main() {

    FILE *file = fopen("test.leyo", "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *buffer = malloc(size + 1);
    if (!buffer) return 1;
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);


    TokenStream ts = tokenise(buffer);
    printTokenStream(ts);

    if (isErr) {
        callAllErr();
    }


    return 0;
}