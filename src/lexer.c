#include "../include/type.h"
#include "../include/liner.h"
#include <stdbool.h>

Token* lex(char *code, int len) {
    char **tokens;
    int tokens_count = 0;
    char *tok;
    int tok_count = 0;
    for (int i; i < len; i++) {
        if (code[i] == ' ') {
            tokens[tokens_count] = tok;
            tokens_count++;
            tok = "";
        } else {
            tok[tok_count] = code[i];
            tok_count++;
        }
    }
}

bool strIn(char c, char *toCheck) {
    for (int i = 0; toCheck[i] != '\0'; i++) {
        if (c == toCheck[i]) { return true; }
    };

    return false;
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

    for (int i = 0; src[i] != '\0'; i++) {
        if (src[i] == '(') {
            tokens[tokenCount] = token(src[i], OPENBRAC, lineNum, i+1);
        } else if (src[i] == ')') {
            tokens[tokenCount] = token(src[i], CLOSEBRAC, lineNum, i+1);
        }
        
    }


    TokenStream tokenstream;
    memcpy(tokenstream.stream, tokens, sizeof(tokens));
    return tokenstream;
}