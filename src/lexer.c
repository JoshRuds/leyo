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

char *charToStr(char c) {
    static char tmp[2];
    tmp[0] = c;
    tmp[1] = '\0';
    return tmp;
}

bool charIn(char c, const char *toCheck) {
    for (int i = 0; toCheck[i] != '\0'; i++) {
        if (c == toCheck[i]) { return true; }
    };

    return false;
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

char previous() {
    return src[l->i-1];
}


void advance() {
    l->i++;
    if (current() == '\n') {
        l->line++;
        l->collumn = 0;
    } else {
        l->collumn++;
    }
    printf("%c\n", current());
}

void handleNormal() {
    char c = current();

    if (isspace(c) || c == '\n') {advance(); return;}

    if (c == '~') {
        l->mode = M_COMMENT;
        printf("cmt mode");
        advance();
        return;
    } else if (c == '"') {
        l->mode = M_STRING;
        printf("str mode");
        advance();
        return;
    } else if (isalpha(c)) {
        l->mode = M_IDENTIFIER;
        printf("idnt mode");
        return;
    } else if (isdigit(c)) {
        l->mode = M_NUMBER;
        printf("num mode");
        return;
    };

    if (c == '(') {push(token(charToStr(c), OPENBRAC));} else
    if (c == ')') {push(token(charToStr(c), CLOSEBRAC));} else
    if (c == ',') {push(token(charToStr(c), COMMA));} else 
    if (c == ';') {push(token(charToStr(c), SEMICOLON));} else
    if (c == '[') {push(token(charToStr(c), OPENSQUARE));} else
    if (c == ']') {push(token(charToStr(c), CLOSESQUARE));} else
    if (c == '{') {push(token(charToStr(c), OPENBRACE));} else
    if (c == '}') {push(token(charToStr(c), CLOSEBRACE));} else
    if (c == '=') {
        if (peek() == '=') {
            advance();
            if (peek() == '=') {
                    advance();
                    push(token("===", CONDITION));
            } else {
                push(token("==", CONDITION));
            }
        } else {
            push(token(charToStr(c), EQUALS));
        }
    } else if (charIn(c, OPERATORS)) {
        push(token(charToStr(c), OPERATION));
    } else if (charIn(c, CONDITIONS)) {
        if (peek() == '=') {
            advance();
            char tmp[3] = { previous(), current(), '\0'};
            push(token(tmp, CONDITION));
        }
        push(token(charToStr(c), CONDITION));
    } else {
        push(token(charToStr(c), UNKNOWN));
        char buffer[48];
        snprintf(buffer, sizeof(buffer), "Invalid Character: %c", current());
        raise(buffer, l->line, l->collumn);
    }

    advance();
    return;
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
            free(buff);
            l->mode = M_NORMAL;
            return;
        }

        if (current() == '"') {
            buff[buffSize] = '\0';
            push(token(buff, STRING));
            l->mode = M_NORMAL;
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
    advance();
    free(buff);
    return;
}

void handleIdentifier() {

    int buffSize = 0;
    int buffCap = 32;
    char *buff = malloc(buffCap);

    if (!buff) {
        raise("Out of memory", l->line, l->collumn);
        return;
    }

    while (true) {
        char c = current();
        if (!isalnum(c) && c != '_') {

            buff[buffSize] = '\0';
            push(token(buff, IDENTIFIER));

            l->mode = M_NORMAL;
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

        buff[buffSize++] = c;
        advance();
    }
    advance();
    free(buff);
}

void handleNumber() {

    int buffSize = 0;
    int buffCap = 32;
    char *buff = malloc(buffCap);

    if (!buff) {
        raise("Out of memory", l->line, l->collumn);
        return;
    }

    bool dotSeen = false;

    while (true) {

        char c = current();
        if (!isdigit(c) && c != '.') {

            buff[buffSize] = '\0';
            push(token(buff, NUMBER));

            l->mode = M_NORMAL;
            break;
        }
        if (c == '.') {
            if (dotSeen) {
                raise("Invalid number format", l->line, l->collumn);
                break;
            }
            dotSeen = true;
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
        buff[buffSize++] = c;
        advance();
    }
    free(buff);
}

void handleComment() {
    while (true) {
        if (current() == '\n' || current() == '~') {
            l->mode = M_NORMAL;
            advance();
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
    
    while (current() != '\0') {
        switch (l->mode) {
            case M_NORMAL:
                handleNormal();
                break;

            case M_STRING:
                handleString();
                break;

            case M_IDENTIFIER:
                handleIdentifier();
                break;

            case M_NUMBER:
                handleNumber();
                break;

            case M_COMMENT:
                handleComment();
                break;
        }
    }
    return lexRes;
}