/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file vm.h
/// @brief Header for the VM.

#ifndef VM_H
#define VM_H

#include "../include/parser.h"

int runVM(ByteCodeResult bc, bool verbose, char filename[512]); 

#endif
