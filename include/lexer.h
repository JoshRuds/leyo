#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#include "type.h"


bool charIn(char c, const char *toCheck);

bool strIn(const char *c, const char **toCheck);

bool isAlpha(char src);

bool isInt(char src);


Token token(const char *value, TokenType type, int line, int column);

TokenStream tokenise(char *src);

#endif