#include "../include/type.h"
#include "../include/liner.h"
#include <stdbool.h>
#include <ctype.h>

bool strIn(char c, char *toCheck) {
    for (int i = 0; toCheck[i] != '\0'; i++) {
        if (c == toCheck[i]) { return true; }
    };

    return false;
}

bool isAlpha(char src) {
    return (toupper(src) != tolower(src));
};

bool isInt(char src) {
    return isdigit(src);
}

Token token(char* value, TokenType type, int line, int collumn) {
    Token t;
    t.value = value;
    t.type = type;
    t.line = line;
    t.collumn = collumn;
    return t;
}

TokenStream tokenise(char* src, int lineNum) {
    Token tokens[4096];
    int tokenCount = 0;

    char ident[64] = "";
    bool isIdentNum = false;

    for (int i = 0; src[i] != '\0'; i++) {
        if (src[i] == '(') {
            tokens[tokenCount] = token(src[i], OPENBRAC, lineNum, i+1);
            tokenCount++;
        } else if (src[i] == ')') {
            tokens[tokenCount] = token(src[i], CLOSEBRAC, lineNum, i+1);
            tokenCount++;
        } else if (strIn(src[i], OPERATORS)) {
            tokens[tokenCount] = token(src[i], OPERATION, lineNum, i+1);
            tokenCount++;
        } else if (src[i] == '=') {
            tokens[tokenCount] = token(src[i], EQUALS, lineNum, i+1);
            tokenCount++;
        } else {

        }
        
    }


    TokenStream tokenstream;
    memcpy(tokenstream.stream, tokens, sizeof(tokens));
    return tokenstream;
}