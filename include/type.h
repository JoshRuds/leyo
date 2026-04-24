#ifndef TYPE_H
#define TYPE_H

typedef enum {
    VAL_NONE,
    VAL_INT,
    VAL_FLOAT,
    VAL_STRING,
    VAL_CHAR,
    VAL_KEYWORD,
    VAL_RESERVED
} ValueType;

typedef struct {
    ValueType type;

    union {
        int i;
        double f;
        char *s;
        char c;
    } value;

    int line;
    int collumn;
    
} Value;

#endif