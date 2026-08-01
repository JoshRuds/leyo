/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file headerer.h
/// @brief Header for the bytecode-headerer.

#ifndef HEADERER_H
#define HEADERER_H

#include "../include/parser.h"

/// @brief The header found at the top of LYBC files.
typedef struct {
    char magic[4];
    char version[64];
    uint16_t flags;
    uint64_t code_size;
    uint64_t globalAmount;
} LeyoHeader;

/// @brief A helper to add a header to parsed bytecode.
/// @param bcr The un-headered bytecode.
/// @return The bytecode with a header.
ByteCodeResult headThis(ByteCodeResult bcr);

#endif
