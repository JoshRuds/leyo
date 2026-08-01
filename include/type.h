/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file type.h
/// @brief Header for the token-based aspects of lexing and parsing.

#ifndef TYPE_H
#define TYPE_H

typedef enum {
    T_NONE,

    // Literals
    T_NUMBER,
    T_FLOAT,
    T_CHAR,
    T_STRING,

    // Identifiers
    T_IDENTIFIER,

    // Assignment
    T_EQUALS,              // =

    // Compound assignment
    T_PLUS_EQUALS,         // +=
    T_MINUS_EQUALS,        // -=
    T_MULTIPLY_EQUALS,     // *=
    T_EXPODENT_EQUALS,     // ^=
    T_DIVIDE_EQUALS,       // /=
    T_MODULO_EQUALS,       // %=

    // Arithmetic
    T_PLUS,                // +
    T_MINUS,               // -
    T_MULTIPLY,            // *
    T_EXPODENT,            // ^
    T_DIVIDE,              // /
    T_MODULO,              // %

    // Comparison
    T_LESS,                // <
    T_LESS_EQUAL,          // <=
    T_GREATER,             // >
    T_GREATER_EQUAL,       // >=
    T_EQUAL_EQUAL,         // ==
    T_NOT_EQUAL,           // !=
    T_STRICT_EQUAL_EQUAL,  // ===
    T_STRICT_NOT_EQUAL,    // !==

    // Logical
    T_LOGICAL_AND,         // &
    T_LOGICAL_OR,          // |
    T_LOGICAL_NOT,         // !

    // Increments
    T_INCREMENT,           // ++
    T_DECREMENT,           // --

    // Brackets
    T_OPENBRAC,            // (
    T_CLOSEBRAC,           // )

    T_OPENBRACE,           // {
    T_CLOSEBRACE,          // }

    T_OPENSQUARE,          // [
    T_CLOSESQUARE,         // ]

    // Punctuation
    T_SEMICOLON,           // ;
    T_COMMA,               // ,
    T_COLON,               // :

    // Special
    T_NATIVE,              // @

    // End / error
    T_ENDOFSTREAM,
    T_UNKNOWN

} TokenType;

typedef struct {
    TokenType type;
    char *value;

    int line;
    int collumn;
} Token;

typedef struct {
    Token *stream;
    int capacity;
    int count;
} TokenStream;

#endif
