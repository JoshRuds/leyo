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


Parser parser;
Parser *p;
Program program;


static Token current() {
    return p->tokens[p->pos];
}

static Token peek() {
    return p->tokens[p->pos + 1];
}

static void advance() {
    p->pos++;
}

static void push() {

}


Program parse(TokenStream tokens) {
    p = &parser;
    p->tokens = tokens.stream;
    p->count = tokens.count;
    p->pos = 0;
    
    while (current().type != ENDOFSTREAM) {

    }

    return program;
}