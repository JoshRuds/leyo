/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file lexer.c
/// @brief The lexer.

#include "../include/type.h"
#include "../include/errors.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    M_COMMENT,
    M_LINE_COMMENT
} LexerMode;

/// @brief The lexer object. Describes lexer related items.
typedef struct {
    int i;
    int line;
    int collumn;
    int scol;
    char filename[512];
    LexerMode mode;
} Lexer;

Lexer lexer;
Lexer *l = &lexer;

TokenStream lexRes = {0};

Token _token(const char *value, TokenType type, int line, int collumn) {
    Token t;
    t.value = strdup(value);
    t.type = type;
    t.line = line;
    t.collumn = collumn;
    return t;
}

Token token(const char *value, TokenType type) {
    return _token(value, type, l->line, l->scol);
}

/// @brief Check the token stream capacity, and re-allocate if low.
/// @return Bool - true if success, else false. 
static bool checkTokenStream(void) {
    if (lexRes.count < lexRes.capacity)
        return true; // No realloc needed

    int newCapacity = lexRes.capacity == 0 ? 256 : lexRes.capacity * 2;

    Token *newStream = realloc(lexRes.stream, newCapacity * sizeof(Token));

    if (!newStream)
        return false;

    lexRes.stream = newStream;
    lexRes.capacity = newCapacity;

    return true;
}

/// @brief Pushes the given token to the lexRes object.
/// @param token The token to push.
static void push(Token token) {
    checkTokenStream();
    logBuildLexer("Token pushed to stream");
    if (lexRes.count >= lexRes.capacity-1) {
        lexRes.capacity *= 2;
        lexRes.stream = realloc(lexRes.stream, lexRes.capacity);
        if (!lexRes.stream) {
            lraise(WF_BUILD, ERR_LEX_OUT_OF_MEMORY, l->line, l->collumn, l->filename);
            return;
        }
    }
    l->scol = l->collumn;
    lexRes.stream[lexRes.count++] = token;
}

/// @brief Gets the next character.
/// @return The next char.
static char peek(void) {
    return src[l->i+1];
}

/// @brief Gets the second next character.
/// @return The second next char.
static char peek2(void) {
    return src[l->i+2];
}

/// @brief Gets the current character.
/// @return The current char.
static char current(void) {
    return src[l->i];
}

/// @brief Gets the previous lexer character.
/// @return The previous char.
static char previous(void) {
    return src[l->i-1];
}

/// @brief Moves on the lexer by one char.
static void advance(void) {
    l->i++;
    if (current() == '\n') {
        l->line++;
        l->collumn = 0;
        l->scol = 0;
    } else {
        l->collumn++;
    }
}

/// @brief Default lex mode - handles all.
static void handleNormal(void) {
    char c = current();

    logBuildLexer("Entering NORMAL mode");

    if (isspace(c) || c == '\n') { advance(); return; }

    if (c == '/' && peek() == '/') {
        l->mode = M_LINE_COMMENT;
        logBuildLexer("Switching to LINE COMMENT mode");
        advance();
        advance();
        return;
    }
    else if (c == '/' && peek() == '*') {
        l->mode = M_COMMENT;
        logBuildLexer("Switching to MULTI-LINE COMMENT mode");
        advance();
        advance();
        return;
    } 
    else if (c == '"') {
        l->mode = M_STRING;
        logBuildLexer("Switching to STRING mode");
        advance();
        return;
    } 
    else if (c == '\'' && peek2() == '\'') {
        logBuildLexer("Handling Char");
        advance();
        push(token(charToStr(current()), T_CHAR));
        advance();
        advance();
        return;
    } 
    else if (isalpha(c) || c == '_') {
        l->mode = M_IDENTIFIER;
        logBuildLexer("Switching to IDENTIFIER mode");
        return;
    } 
    else if (isdigit(c)) {
        l->mode = M_NUMBER;
        logBuildLexer("Switching to NUMBER mode");
        return;
    };

    // Singles
    if (c == '@') {
        push(token(charToStr(c), T_NATIVE));
    } else if (c == '(') {
        push(token(charToStr(c), T_OPENBRAC));
    } else if (c == ')') {
        push(token(charToStr(c), T_CLOSEBRAC));
    } else if (c == ',') {
        push(token(charToStr(c), T_COMMA));
    } else if (c == ';') {
        push(token(charToStr(c), T_SEMICOLON));
    } else if (c == '[') {
        push(token(charToStr(c), T_OPENSQUARE));
    } else if (c == ']') {
        push(token(charToStr(c), T_CLOSESQUARE));
    } else if (c == '{') {
        push(token(charToStr(c), T_OPENBRACE));
    } else if (c == '}') {
        push(token(charToStr(c), T_CLOSEBRACE));
    } else if (c == ':') {
        push(token(charToStr(c), T_COLON));
    } else if (c == '&') {
        push(token(charToStr(c), T_LOGICAL_AND));
    } else if (c == '|') {
        push(token(charToStr(c), T_LOGICAL_OR));

    // Assignment / equality 
    } else if (c == '=') {
        if (peek() == '=') {
            advance();

            if (peek() == '=') {
                advance();
                push(token("===", T_STRICT_EQUAL_EQUAL));
            } else {
                push(token("==", T_EQUAL_EQUAL));
            }
        } else {
            push(token(charToStr(c), T_EQUALS));
        }

    // Not / not equal 
    } else if (c == '!') {
        if (peek() == '=') {
            advance();

            if (peek() == '=') {
                advance();
                push(token("!==", T_STRICT_NOT_EQUAL));
            } else {
                push(token("!=", T_NOT_EQUAL));
            }
        } else {
            push(token(charToStr(c), T_LOGICAL_NOT));
        }

    // Plus / increment / plus assignment 
    } else if (c == '+') {
        if (peek() == '+') {
            advance();
            push(token("++", T_INCREMENT));
        } else if (peek() == '=') {
            advance();
            push(token("+=", T_PLUS_EQUALS));
        } else {
            push(token(charToStr(c), T_PLUS));
        }

    // Minus / decrement / minus assignment 
    } else if (c == '-') {
        if (peek() == '-') {
            advance();
            push(token("--", T_DECREMENT));
        } else if (peek() == '=') {
            advance();
            push(token("-=", T_MINUS_EQUALS));
        } else {
            push(token(charToStr(c), T_MINUS));
        }

    // Multiply / multiply assignment 
    } else if (c == '*') {
        if (peek() == '=') {
            advance();
            push(token("*=", T_MULTIPLY_EQUALS));
        } else {
            push(token(charToStr(c), T_MULTIPLY));
        }

    // Divide / divide assignment 
    } else if (c == '/') {
        if (peek() == '=') {
            advance();
            push(token("/=", T_DIVIDE_EQUALS));
        } else {
            push(token(charToStr(c), T_DIVIDE));
        }

    // Exponent / exponent assignment 
    } else if (c == '^') {
        if (peek() == '=') {
            advance();
            push(token("^=", T_DIVIDE_EQUALS));
        } else {
            push(token(charToStr(c), T_DIVIDE));
        }

    // Modulo / modulo assignment 
    } else if (c == '%') {
        if (peek() == '=') {
            advance();
            push(token("%=", T_MODULO_EQUALS));
        } else {
            push(token(charToStr(c), T_MODULO));
        }

    // Less than / less than or equal 
    } else if (c == '<') {
        if (peek() == '=') {
            advance();
            push(token("<=", T_LESS_EQUAL));
        } else {
            push(token(charToStr(c), T_LESS));
        }

    // Greater than / greater than or equal 
    } else if (c == '>') {
        if (peek() == '=') {
            advance();
            push(token(">=", T_GREATER_EQUAL));
        } else {
            push(token(charToStr(c), T_GREATER));
        }

    // Invalid character 
    } else {
        char buffer[48];

        snprintf(
            buffer,
            sizeof(buffer),
            "Invalid Character: %c",
            current()
        );

        logBuildLexer(buffer);

        lraise(
            WF_BUILD,
            ERR_LEX_INVALID_CHAR,
            l->line,
            l->collumn,
            l->filename
        );

        push(token(charToStr(c), T_UNKNOWN));
    }

    advance();
}

/// @brief Lex mode to handle strings. 
static void handleString(void) {
    logBuildLexer("Entering STRING mode");

    int buffSize = 0;
    int buffCap = 32;
    char *buff = malloc(buffCap);

    if (!buff) {
        logBuildLexer("String buffer allocation failed");
        lraise(WF_BUILD, ERR_LEX_OUT_OF_MEMORY, l->line, l->collumn, l->filename);
        return;
    }

    while (true) {
        if (current() == '\n' || current() == '\0') {
            logBuildLexer("Unterminated string detected");
            lraise(WF_BUILD, ERR_LEX_UNTERMINATED_STR_LIT, l->line, l->collumn, l->filename);
            free(buff);
            l->mode = M_NORMAL;
            return;
        }

        if (current() == '"') {
            buff[buffSize] = '\0';
            push(token(buff, T_STRING));
            l->mode = M_NORMAL;
            break;
        }

        if (buffSize >= buffCap - 1) {
            buffCap *= 2;
            char *tmp = realloc(buff, buffCap);

            if (!tmp) {
                logBuildLexer("String realloc failed");
                free(buff);
                lraise(WF_BUILD, ERR_LEX_OUT_OF_MEMORY, l->line, l->collumn, l->filename);
                return;
            }
            buff = tmp;
        }

        buff[buffSize++] = current();
        advance();
    }

    advance();
    free(buff);
}

/// @brief Lex mode to handle identifiers. 
static void handleIdentifier(void) {
    logBuildLexer("Entering IDENTIFIER mode");

    int buffSize = 0;
    int buffCap = 32;
    char *buff = malloc(buffCap);

    if (!buff) {
        logBuildLexer("Identifier allocation failed");
        lraise(WF_BUILD, ERR_LEX_OUT_OF_MEMORY, l->line, l->collumn, l->filename);
        return;
    }

    while (true) {
        char c = current();

        if (!isalnum(c) && c != '_') {
            buff[buffSize] = '\0';
            push(token(buff, T_IDENTIFIER));
            l->mode = M_NORMAL;
            break;
        }

        if (buffSize >= buffCap - 1) {
            buffCap *= 2;
            char *tmp = realloc(buff, buffCap);

            if (!tmp) {
                logBuildLexer("Identifier realloc failed");
                free(buff);
                lraise(WF_BUILD, ERR_LEX_OUT_OF_MEMORY, l->line, l->collumn, l->filename);
                return;
            }
            buff = tmp;
        }

        buff[buffSize++] = c;
        advance();
    }

    //advance();
    free(buff);
}

/// @brief Lex mode to handle numbers. 
static void handleNumber(void) {
    logBuildLexer("Entering NUMBER mode");

    int buffSize = 0;
    int buffCap = 32;
    char *buff = malloc(buffCap);

    if (!buff) {
        logBuildLexer("Number allocation failed");
        lraise(WF_BUILD, ERR_LEX_OUT_OF_MEMORY, l->line, l->collumn, l->filename);
        return;
    }

    bool dotSeen = false;
    TokenType flag = T_NUMBER;

    while (true) {
        char c = current();

        if (!isdigit(c) && c != '.') {
            buff[buffSize] = '\0';
            push(token(buff, flag));
            l->mode = M_NORMAL;
            break;
        }

        if (c == '.') {
            if (dotSeen) {
                logBuildLexer("Invalid number format (multiple dots)");
                lraise(WF_BUILD, ERR_LEX_INVALID_NUM, l->line, l->collumn, l->filename);
                break;
            }
            flag = T_FLOAT;
            dotSeen = true;
        }

        if (buffSize >= buffCap - 1) {
            buffCap *= 2;
            char *tmp = realloc(buff, buffCap);

            if (!tmp) {
                logBuildLexer("Number realloc failed");
                free(buff);
                lraise(WF_BUILD, ERR_LEX_OUT_OF_MEMORY, l->line, l->collumn, l->filename);
                return;
            }
            buff = tmp;
        }

        buff[buffSize++] = c;
        advance();
    }

    free(buff);
}

/// @brief Handles one line comments. 
static void handleLineComment(void) {
    logBuildLexer("Entering COMMENT mode");

    while (true) {
        if (current() == '\n') {
            l->mode = M_NORMAL;
            advance();
            break;
        } else {
            advance();
        }
    }
}

/// @brief Handles multiline comments. 
static void handleComment(void) {
    logBuildLexer("Entering COMMENT mode");

    while (true) {
        if (current() == '*' && peek() == '/') {
            l->mode = M_NORMAL;
            advance();
            advance();
            break;
        } else {
            advance();
        }
    }
}

TokenStream tokenise(char* _src) {
    src = _src;

    lexRes.count = 0;
    lexRes.capacity = 4096;
    lexRes.stream = malloc(sizeof(Token) * lexRes.capacity);

    l->i = 0;
    l->collumn = 1;
    l->scol = 1;
    l->line = 1;
    l->mode = M_NORMAL;

    logBuildLexer("Lexer started");

    while (current() != '\0') {
        switch (l->mode) {
            case M_NORMAL: handleNormal(); break;
            case M_STRING: handleString(); break;
            case M_IDENTIFIER: handleIdentifier(); break;
            case M_NUMBER: handleNumber(); break;
            case M_COMMENT: handleComment(); break;
            case M_LINE_COMMENT: handleLineComment(); break;
        }
    }

    push(token("EndOfStream", T_ENDOFSTREAM));
    logBuildLexer("Lexer finished");
    return lexRes;
}
