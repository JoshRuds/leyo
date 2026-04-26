#include "../include/type.h"
#include "../include/errors.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

const char OPERATORS[] = "+-*/^";
const char CONDITIONS[] = "><!";

char *src;


bool isAlpha(char src) {
    return (toupper(src) != tolower(src));
}

bool isInt(char src) {
    return isdigit(src);
}

typedef enum {
    NORMAL,
    STRING,
    NUMBER,
    IDENTIFIER,
    COMMENT
} LexerMode;

typedef struct {
    int i;
    int line;
    int collumn;
    LexerMode mode;
} Lexer;

Lexer lexer;
Lexer *l = &lexer;

TokenStream lexRes;

Token _token(const char *value, TokenType type, int line, int collumn) {
    Token t;
    t.value = strdup(value);
    t.type = type;
    t.line = line;
    t.collumn = collumn;
    return t;
}

Token token(const char *value, TokenType type) {
    return _token(value, type, l->line, l->collumn);
}

void push(Token token) {
    lexRes.stream[lexRes.count++] = token;
}

char peek() {
    l->i
}

Token handleNormal() {

}

Token handleString() {
    
}

Token handleIdentifier() {
    
}

Token handleNumber() {
    
}

Token handleComment() {
    while (true) {
        if (peek() == )
    }
    
}



TokenStream tokenise(char* _src) {
    src = _src;

    l->i = 0;
    l->collumn = 1;
    l->line = 1;
    l->mode = NORMAL;
    
    while (src[l->i] != '\0') {
        switch (l->mode) {
            case NORMAL:
                handleNormal(src);
                break;

            case STRING:
                handleString(src);
                break;

            case IDENTIFIER:
                handleIdentifier(src);
                break;

            case NUMBER:
                handleNumber(src);
                break;

            case COMMENT:
                handleComment(src);
                break;
        }
    }
}