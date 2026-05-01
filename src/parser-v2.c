#include "../include/type.h"
#include "../include/errors.h"
#include "../include/nodes.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    Token *tokens;
    int pos;
    int count;
} Parser;

char *VARDEFS[] = {
    "str",
    "int",
    "flt",
};
int VARDEFCOUNT = 3;

Parser parser = {0};
Parser *p;
Program program = {0};

static Token current() {
    return p->tokens[p->pos];
}

static Token peek() {
    return p->tokens[p->pos+1];
}

static void advance() {
    p->pos++;
}

static void expect(TokenType type, char *errorStr) {
    if (type != peek().type) {
        raise(errorStr, peek().line, peek().collumn);
    }
    advance();
}

static void expectAndPass(TokenType type, char *errorStr) {
    if (type != peek().type) {
        raise(errorStr, peek().line, peek().collumn);
    }
    advance();
    advance();
}

static bool strInVarDef(char *str) {
    for (int i = 0; i < VARDEFCOUNT; i++) {
        if (strcmp(VARDEFS[i], str) == 0) {
            return true;
        }
    }
    return false;
}
