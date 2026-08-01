/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file build.c
/// @brief The build handler.

#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/headerer.h"
#include "../include/errors.h"
#include "../include/codes.h"

/// @brief Gets the string representation of a token type.
/// @param t The token type.
/// @return The name of the specified token type, or "???" if the token type is unknown.
static const char *tokenTypeName(TokenType t) {
    switch (t) {
        // General
        case T_NONE: return "NONE";

        // Literals
        case T_NUMBER: return "NUMBER";
        case T_FLOAT: return "FLOAT";
        case T_CHAR: return "CHAR";
        case T_STRING: return "STRING";

        // Identifiers
        case T_IDENTIFIER: return "IDENTIFIER";

        // Assignment
        case T_EQUALS: return "EQUALS";

        // Compound assignment
        case T_PLUS_EQUALS: return "PLUS_EQUALS";
        case T_MINUS_EQUALS: return "MINUS_EQUALS";
        case T_MULTIPLY_EQUALS: return "MULTIPLY_EQUALS";
        case T_EXPODENT_EQUALS: return "EXPODENT_EQUALS";
        case T_DIVIDE_EQUALS: return "DIVIDE_EQUALS";
        case T_MODULO_EQUALS: return "MODULO_EQUALS";

        // Arithmetic
        case T_PLUS: return "PLUS";
        case T_MINUS: return "MINUS";
        case T_MULTIPLY: return "MULTIPLY";
        case T_EXPODENT: return "EXPODENT";
        case T_DIVIDE: return "DIVIDE";
        case T_MODULO: return "MODULO";

        // Comparison
        case T_LESS: return "LESS";
        case T_LESS_EQUAL: return "LESS_EQUAL";
        case T_GREATER: return "GREATER";
        case T_GREATER_EQUAL: return "GREATER_EQUAL";
        case T_EQUAL_EQUAL: return "EQUAL_EQUAL";
        case T_NOT_EQUAL: return "NOT_EQUAL";
        case T_STRICT_EQUAL_EQUAL: return "STRICT_EQUAL_EQUAL";
        case T_STRICT_NOT_EQUAL: return "STRICT_NOT_EQUAL";

        // Logical
        case T_LOGICAL_AND: return "LOGICAL_AND";
        case T_LOGICAL_OR: return "LOGICAL_OR";
        case T_LOGICAL_NOT: return "LOGICAL_NOT";

        // Increments
        case T_INCREMENT: return "INCREMENT";
        case T_DECREMENT: return "DECREMENT";

        // Brackets
        case T_OPENBRAC: return "OPENBRAC";
        case T_CLOSEBRAC: return "CLOSEBRAC";
        case T_OPENBRACE: return "OPENBRACE";
        case T_CLOSEBRACE: return "CLOSEBRACE";
        case T_OPENSQUARE: return "OPENSQUARE";
        case T_CLOSESQUARE: return "CLOSESQUARE";

        // Punctuation
        case T_SEMICOLON: return "SEMICOLON";
        case T_COMMA: return "COMMA";
        case T_COLON: return "COLON";

        // Special
        case T_NATIVE: return "NATIVE";

        // End / error
        case T_ENDOFSTREAM: return "ENDOFSTREAM";
        case T_UNKNOWN: return "UNKNOWN";

        default: return "???";
    }
}

/// @brief Prints the provided token stream.
/// @param ts The token stream object.
static void printTokenStream(TokenStream ts) {
    logController("Printing token stream");

    for (int i = 0; i < ts.count; i++) {
        Token t = ts.stream[i];

        printf("Token %d:\n", i);
        printf("  Type   : %s\n", tokenTypeName(t.type));
        printf("  Value  : %s\n", t.value);
        printf("  Line   : %d\n", t.line);
        printf("  Column : %d\n", t.collumn);
        printf("\n");
    }

    logController("Finished printing token stream");
}

/// @brief Prints the provided bytecode.
/// @param bc The byte code result object.
static void printByteCode(ByteCodeResult* bc) {
    logController("Printing bytecode output");

    for (int i = 0; i < bc->length; i++) {
        printf("%02x ", bc->data[i]);
    }
    printf("\n");

    logController("Finished printing bytecode output");
}

int build(char *filename, char *bcrfilename, bool isFlnameScript, bool dump) {
    logController("Build started");
    char *buffer = NULL;

    if (isFlnameScript) {
        logController("Build Is A Script");
        buffer = filename;
        goto tokenising;
    }

    {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Input=%s Output=%s", filename, bcrfilename);
        logController(buffer);
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        logController("Failed to open input file");
        lraise(WF_GENERAL, ERR_FILE_OPEN_ERROR, 0, 0, NULL);
        return 1;
    }

    logController("Input file opened successfully");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    
    buffer = malloc(size + 1);
    if (!buffer) {
        logController("Memory allocation failed for file buffer");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);

    logController("File loaded into memory");

tokenising:
    ;
    TokenStream ts = tokenise(buffer);
    logController("Tokenisation completed");

    if (dump) {
        printTokenStream(ts);
    }
    
    if (isErr) {
        logController("Errors detected after tokenisation");
        callAllErr();
    }

    ByteCodeResult bcr = headThis(parse(&ts, filename));

    logController("Parsing to bytecode completed");

    if (!(bcr.data == NULL || bcr.length == 0)) {
        logController("Bytecode successfully generated");
    } else {
        logController("Bytecode generation failed or empty");
    }

    if (dump) {
        printByteCode(&bcr);
    }

    if (isErr) {
        logController("Errors detected after building. Exiting.");
        return 1;
    }
    
    logController("Program built successfully");

    FILE* filebcr = fopen(bcrfilename, "wb");

    if (!filebcr) {
        logController("Fail to open bcr file");
        lraise(WF_GENERAL, ERR_FILE_OPEN_ERROR, 0,0, NULL);
    }

    fwrite(bcr.data, 1, bcr.length, filebcr);

    fclose(filebcr);

    return 0;
}