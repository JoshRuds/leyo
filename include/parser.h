/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file parser.h
/// @brief Header for the parser.

#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include "../include/type.h"

/// @brief A global object - contains a slot for where in the global array it is.
typedef struct {
    char *name;
    uint16_t slot;
    TokenType type;
} Global;

/// @brief A flag for the value union.
typedef enum {
    VAL_FLOAT,
    VAL_INT,
    VAL_STR,
    VAL_CHAR,
} ValueFlag;

/// @brief The value union. Describes how leyo values are to be held.
/// @note String is to be sterilised later.
typedef union {
    int i;
    double f;
    char c;
    char *s;
} ValueTypes;

/// @brief Describes a value for constants.
typedef struct {
    ValueFlag flag;
    ValueTypes as;
} Value;

/// @brief Descibes a function to be stored.
typedef struct {
    char *name;
    uint32_t address;
    TokenType retType;
    // todo add args 
} Func;

/// @brief Bytecoder object - contains all data that the parser needs.
/// @note Internal only.
typedef struct {
    Token *tokens;
    uint32_t pos;
    uint32_t count;

    uint8_t *bytebuff;
    uint32_t byteIndex;
    uint32_t byteCap;

    Global globals[65535];
    int globalCount;

    Value *consts;
    uint64_t constAmt;

    Func *funcs;
    int funcAmt;

    char funcPrefix[256];

    char **modulesLoaded;
    int moduleAmt;
    int moduleCap;

    char currentFileName[512];
} ByteCoder;

/// @brief A buffer for constants.
typedef struct {
    uint8_t *data;
    int length;
} ConstBuffer;

/// @brief The result to be passed between processes.
typedef struct {
    uint8_t *data;
    int length;
    ConstBuffer cb;
} ByteCodeResult;

/// @brief The entry point to parsing.
/// @param ts A tokenstream from the tokeniser.
/// @param currentFileName The filename. Used for errors and dependancies.
/// @return A bytecoderesult object to be headered then stored.
ByteCodeResult parse(TokenStream *ts, char *currentFileName);

#endif
