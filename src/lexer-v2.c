#include "../include/type.h"
#include "../include/errors.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    M_NORMAL,
    M_STRING,
    M_NUMBER,
    M_IDENTIFIER,
    M_COMMENT
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
    return src[l->i+1];
}

char current() {
    return src[l->i];
}

void advance() {
    l->i++;
    if (current() == '\n') {
        l->line++;
        l->collumn = 0;
    } else {
        l->collumn++;
    }
}

void handleNormal() {
    if (current() == '~') {
        l->mode = M_COMMENT;
        advance();
        return;
    } else if (current() == '"') {
        l->mode = M_STRING;
        advance();
        return;
    }
}

void handleString() {
    
    int buffSize = 0;
    int buffCap = 32;
    char *buff = malloc(buffCap);
    if (!buff) {
        raise("Out of memory", l->line, l->collumn);
        return;
    }

    while (true) {
        if (current() == '\n' || current() == '\0') {
            raise("Unterminated String Literal", l->line, l->collumn);
            break;
        }

        if (current() == '"') {
            buff[buffSize] = '\0';
            push(token(buff, M_STRING));
            break;
        }


        if (buffSize >= buffCap - 1) {
            buffCap *= 2;
            char *tmp = realloc(buff, buffCap);

            if (!tmp) {
                free(buff);
                raise("Out of memory", l->line, l->collumn);
                return;
            }
            buff = tmp;
        }
        buff[buffSize++] = current();
        advance();
    }
    free(buff);
    return;
}

void handleIdentifier() {
    return;
}

void handleNumber() {
    return;
}

void handleComment() {
    while (true) {
        if (current() == '\n' || current() == '~') {
            l->mode = M_NORMAL;
            break;
        } else {
            advance();
        }
    }
    return;
}



TokenStream tokenise(char* _src) {
    src = _src;

    l->i = 0;
    l->collumn = 1;
    l->line = 1;
    l->mode = M_NORMAL;
    
    while (src[l->i] != '\0') {
        switch (l->mode) {
            case M_NORMAL:
                handleNormal(src);
                break;

            case M_STRING:
                handleString(src);
                break;

            case M_IDENTIFIER:
                handleIdentifier(src);
                break;

            case M_NUMBER:
                handleNumber(src);
                break;

            case M_COMMENT:
                handleComment(src);
                break;
        }
    }
    return lexRes;
}