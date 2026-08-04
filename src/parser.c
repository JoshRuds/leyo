/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file parser.c
/// @brief The parser.

#include <stdlib.h>
#include "../include/type.h"
#include "../include/parser.h"
#include "../include/errors.h"

/// @brief The internal only parser object.
typedef struct {
    Token *tokens;
    size_t tp; // token pointer
    size_t tokCount;
} Parser;

Parser parser = {0}; 
Parser *p; // set to parser 

ASTNodeList programSTD = {0};
ASTNodeList *program; // set to ProgramSTD


/// @brief Get the current token.
/// @return The token at the current parser position.
static Token current(void) {
    return p->tokens[p->tp];
}

static void push(ASTNode *node) {
    if (program->count >= program->capacity) {
        size_t newCapacity = program->capacity == 0
            ? 8
            : program->capacity * 2;

        ASTNode **newNodes = realloc(
            program->nodes,
            newCapacity * sizeof(*newNodes)
        );

        if (newNodes == NULL) {
            lraise(WF_BUILD, ERR_PARSER_CANNOT_ALLOCATE, )
        }

        program->nodes = newNodes;
        program->capacity = newCapacity;
    }

    program->nodes[program->count++] = node;
}

ASTNodeList parse(TokenStream ts) {
    p = &parser;
    program = &programSTD;
}