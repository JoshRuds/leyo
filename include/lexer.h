/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file lexer.h
/// @brief Header for the lexer.

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#include "type.h"

/// @brief Helper - checks if char is present in a string.
/// @param c The char to search for.
/// @param toCheck What to search in.
/// @return Bool - true if present, false if not.
bool charIn(char c, const char *toCheck);

/// @brief Checks if the character is alphabetic.
/// @param src The char to check.
/// @return Bool - true if @p src is alpha, else false.
bool isAlpha(char src);

/// @brief Checks if the character is a digit.
/// @param src The char to check.
/// @return Bool - true if @p src is a digit, else false.
bool isInt(char src);

/// @brief Generates a Token object.
/// @param value The value of the token.
/// @param type The TokenType.
/// @param line The line number.
/// @param column The column.
/// @return The constructed Token.
Token _token(const char *value, TokenType type, int line, int column);

/// @brief Generates a Token object.
/// @param value The value of the token.
/// @param type The TokenType.
/// @note Removes need for line and column and uses the Lexer's line and column values.
/// @return The constructed Token.
Token token(const char *value, TokenType type);

/// @brief The handler for tokenising.
/// @param src The contents to tokenise.
/// @return The TokenStream output.
TokenStream tokenise(char *src);

#endif
