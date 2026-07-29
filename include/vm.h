/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file vm.h
/// @brief Header for the VM.

#ifndef VM_H
#define VM_H

#include "../include/parser.h"

/// @brief Run the VM on the given params.
/// @param bc The loaded bytecoderesult object.
/// @param verbose Describes how much information to output.
/// @param filename The filename for errors purposes.
/// @retval 0 Success.
/// @retval 1 Failure.
int runVM(ByteCodeResult bc, bool verbose, char filename[512]); 

#endif
