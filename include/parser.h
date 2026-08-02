/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file parser.h
/// @brief Header for the parser.

#ifndef PARSER_H
#define PARSER_H

#include "../include/type.h"

typedef enum {
    AST_BINARY,
    AST_ASSIGN,
    AST_LITERAL,
    AST_VARIABLE,
} ASTNodeType; 

typedef struct ASTNode ASTNode;

typedef union {
    int literal;

    struct {
        ASTNode *left;
        ASTNode *right;
        TokenType operator;
    } binary;

    char *variable;

    struct {
        char *name;
        ASTNode *value;
    } assignment;
} ASTData;

struct ASTNode {
    ASTNodeType type;
    ASTData data;
};

typedef struct {
    ASTNode **nodes;
    size_t count;
    size_t capacity;
} ASTNodeList;

#endif
