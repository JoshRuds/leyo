#include <stdio.h>
#include "../include/liner.h"
#include "../include/lexer.h"

void printTokenStream(TokenStream ts) {
    for (int i = 0; i < ts.count; i++) {
        printf("TOKEN GENERATED\n");
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
    char code[] = "int main = (5+35*6); log main";

    int len;  // FIX: was uninitialised pointer

    char **splited = lineSplit(code, &len);  // FIX: pass address properly
    printf("Len = %d\n", len);
    for (int i = 0; i < len; i++) {  // FIX: no pointer dereference
        TokenStream ts = tokenise(splited[i], i + 1);
        printTokenStream(ts);
    }

    return 0;
}