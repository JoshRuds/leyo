/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file lyst.h
/// @brief Header for the LYST configurator.

#ifndef LYST_H
#define LYST_H

#include <stdbool.h>

#define LYST_MAX_ENTRIES 256

/// @brief Describes a key-value pair in the LYST file.
typedef struct {
    char section[64];
    char key[64];
    char value[256];
} LystEntry;

/// @brief Describes a LYST file.
typedef struct {
    LystEntry entries[LYST_MAX_ENTRIES];
    int count;
} LystConfig;

/// @brief Load a LYST file.
/// @param filename The file to open.
/// @retval 0 Pass - no LYST.
/// @retval 1 Pass - LYST loaded.
int lystLoad(const char *filename);

/// @brief Gets the value from the given key.
/// @param path The key to search for.
/// @return The value.
const char *lystGet(const char *path);

/// @brief Gets the boolean value from the given key.
/// @param path The key to search for.
/// @param fallback Result if no bool provided.
/// @return The boolean value.
bool lystGetBool(const char *path, bool fallback);

/// @brief Gets the integer from the given key.
/// @param path The key to search for.
/// @param fallback Result if no int provided.
/// @return The integer value.
int lystGetInt(const char *path, int fallback);

#endif
