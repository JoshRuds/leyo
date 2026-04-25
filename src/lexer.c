#include "../include/type.h"
#include "../include/liner.h"
#include <stdbool.h>
#include <ctype.h>

bool charIn(char c, char *toCheck) {
    for (int i = 0; toCheck[i] != '\0'; i++) {
        if (c == toCheck[i]) { return true; }
    };

    return false;
}

bool strIn(char *c, char *toCheck) {
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
    int identCount = 0;

    int stateSaveI;

    for (int i = 0; src[i] != '\0'; i++) {
        if (src[i] == '(') {
            tokens[tokenCount] = token(src[i], OPENBRAC, lineNum, i+1);
            tokenCount++;
        } else if (src[i] == ')') {
            tokens[tokenCount] = token(src[i], CLOSEBRAC, lineNum, i+1);
            tokenCount++;
        } else if (charIn(src[i], OPERATORS)) {
            tokens[tokenCount] = token(src[i], OPERATION, lineNum, i+1);
            tokenCount++;
        } else if (src[i] == '=') {
            tokens[tokenCount] = token(src[i], EQUALS, lineNum, i+1);
            tokenCount++;
        } else {
            stateSaveI = i;
            if (isInt(src[i])) {
                for (; src[i] != '\0' && isInt(src[i]); i++) {
                    ident[identCount] = src[i];
                };
                tokens[tokenCount] = token(ident, NUMBER, lineNum, stateSaveI++);
                tokenCount++;
            } else if (isAlpha(src[i])) {
                for (; src[i] != '\0' && isAlpha(src[i]); i++) {
                    ident[identCount] = src[i];
                };

                if (strIn(ident, VARDEFS)) {
                    tokens[tokenCount] = token(ident, VARDEFS, lineNum, stateSaveI++);
                } else {
                    tokens[tokenCount] = token(ident, IDENTIFIER, lineNum, stateSaveI++);
                    tokenCount++;
                };
                
            }
            i--;
        }
        
    }


    TokenStream tokenstream;
    memcpy(tokenstream.stream, tokens, sizeof(tokens));
    return tokenstream;
}