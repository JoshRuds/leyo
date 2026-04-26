#include "../include/type.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

const char OPERATORS[] = "+-*/^";
const char *VARDEFS[] = {"int", "char", "bool", "string"};


static inline bool isSkippable(char c) {
    return c == ' ' || c == '\t';
}


bool charIn(char c, const char *toCheck) {
    for (int i = 0; toCheck[i] != '\0'; i++) {
        if (c == toCheck[i]) { return true; }
    };

    return false;
}

bool strIn(const char *c, const char **toCheck) {
    for (int i = 0; toCheck[i] != NULL; i++) {
        if (strcmp(c, toCheck[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isAlpha(char src) {
    return (toupper(src) != tolower(src));
}

bool isInt(char src) {
    return isdigit(src);
}

Token token(const char *value, TokenType type, int line, int collumn) {
    Token t;
    t.value = strdup(value);
    t.type = type;
    t.line = line;
    t.collumn = collumn;
    return t;
}

TokenStream tokenise(char* src) {
    Token tokens[4096];
    int tokenCount = 0;

    char ident[256];
    int identCount = 0;

    int stateSaveI;
    int lineNum = 1;

    for (int i = 0; src[i] != '\0'; i++) {

        if (src[i] == '\n') {lineNum++; continue;};

    
        if (isSkippable(src[i])) {
            continue;
        }

        printf("i=%d char=%c\n", i, src[i]);
        if (src[i] == '(') {
            char tmp[2] = { src[i], '\0' }; 
            tokens[tokenCount++] = token(tmp, OPENBRAC, lineNum, i+1);
        } else if (src[i] == ')') {
            char tmp[2] = { src[i], '\0' }; 
            tokens[tokenCount++] = token(tmp, CLOSEBRAC, lineNum, i+1);
        } else if (charIn(src[i], OPERATORS)) {
            char tmp[2] = { src[i], '\0' }; 
            tokens[tokenCount++] = token(tmp, OPERATION, lineNum, i+1);
        } else if (src[i] == '=') {
            char tmp[2] = { src[i], '\0' }; 
            tokens[tokenCount++] = token(tmp, EQUALS, lineNum, i+1);
        } else {
            stateSaveI = i;
            if (isInt(src[i])) {
                identCount = 0;
                memset(ident, 0, sizeof(ident));
                for (; src[i] != '\0' && isInt(src[i]); i++) {
                    ident[identCount++] = src[i];
                    if (src[i+1] != '\0' && isAlpha(src[i+1])) {
                        continue;
                    } else {
                        break;
                    }
                };
                tokens[tokenCount++] = token(ident, NUMBER, lineNum, stateSaveI++);
                ident[identCount] = '\0';
            } else if (isAlpha(src[i])) {
                identCount = 0;
                memset(ident, 0, sizeof(ident));
                for (; src[i] != '\0' && isAlpha(src[i]); i++) {
                    ident[identCount++] = src[i];
                    if (src[i+1] != '\0' && isAlpha(src[i+1])) {
                        continue;
                    } else {
                        break;
                    }
                };
                ident[identCount] = '\0';

                tokens[tokenCount++] = token(ident, IDENTIFIER, lineNum, stateSaveI++);
                continue;
                if (strIn(ident, VARDEFS)) {
                    tokens[tokenCount++] = token(ident, VARIABLEDEF, lineNum, stateSaveI++);
                } else {
                    tokens[tokenCount++] = token(ident, IDENTIFIER, lineNum, stateSaveI++);
                };
            } else {
                err
            }
        }
        
    }


    TokenStream tokenstream;
    memcpy(tokenstream.stream, tokens, tokenCount * sizeof(Token));
    tokenstream.count = tokenCount;
    return tokenstream;
}