#include "../include/type.h"
#include "../include/liner.h"

Token lex(char *code, int len) {
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