/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file parser.c
/// @brief The parser.

#include <stdlib.h>
#include "../include/type.h"
#include "../include/parser.h"

typedef struct {
    Token *tokens;
    size_t tokCapacity;
    size_t tokCount;
} Parser;

Parser parser = {0};
Parser *p; // set to parser
