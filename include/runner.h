/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file runner.h
/// @brief Header for the runner handler.

#ifndef RUNNER_H
#define RUNNER_H

/// @brief Run the given program.
/// @param filename The file to run.
/// @param verbose Describes how much to output.
/// @retval 0 Success.
/// @retval 1 Failure.
int run(char *filename, bool verbose);

#endif
