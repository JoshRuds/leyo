#ifndef TYPE_H
#define TYPE_H

typedef enum {
    NONE,
    NUMBER,
    IDENTIFIER,
    EQUALS,
    OPENBRAC,
    CLOSEBRAC,
    OPERATION,
    VARIABLEDEF,
    KEYWORD,
    RESERVED
} TokenType;

typedef struct {
    TokenType type;
    char *value;

    int line;
    int collumn;

} Token;

#endif