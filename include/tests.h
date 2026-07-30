/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file tests.h
/// @brief Header for the testing suite.

#ifndef TESTS_H
#define TESTS_H

/// @brief Test leyo by running test programs.
/// @param leyoBin The location of the leyo binary.
/// @retval 0 Success.
/// @retval 1 Failure.
int testLeyo(const char *leyoBin);

#endif
