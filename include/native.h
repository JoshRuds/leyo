/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file native.h
/// @brief Header for native functions.

#ifndef NATIVE_H
#define NATIVE_H

/// @brief The native commands used in parser and VM.
typedef enum {
    NAT_LOG,
    NAT_DUMP,
    NAT_TRACE,
    NAT_PRINT,
    NAT_UNKOWN,
    NAT_EXIT,
} NativeCommand;

#endif
