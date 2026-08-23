/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file parser.h
/// @brief Header for the parser.

#ifndef PARSER_H
#define PARSER_H

#include "type.h"

/// @brief The types of Nodes in Leyo's AST.
typedef enum {
    AST_BINARY,
    AST_ASSIGN,
    AST_LITERAL,
    AST_VARIABLE,
} ASTNodeType; 

typedef struct ASTNode ASTNode;

/// @brief The data that can be held by the Node in Leyo's AST.
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

/// @brief An AST Node for Leyo.
struct ASTNode {
    ASTNodeType type;
    ASTData data;
};

/// @brief A list of AST Nodes. This can be used for storing the program.
typedef struct {
    ASTNode **nodes;
    size_t count;
    size_t capacity;
} ASTNodeList;

ASTNodeList parse(TokenStream ts);

#endif
