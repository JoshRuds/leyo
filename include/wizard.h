/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file wizard.h
/// @brief Header for the LYST wizard.

#ifndef WIZARD_H
#define WIZARD_H

#include "../include/errors.h"
#include "../include/codes.h"
#include "../include/codes.h"

/// @brief Reads from the `.lyst` file and returns its contents as an object.
/// @return A LogConfig object for the `.lyst` file.
LogConfig readLogConfig(void);

/// @brief Writes the default LYST to the`.lyst` file.
/// @return `true` if the LYST file was successfully written; `false` if the file could not be opened.
bool writeDefaultLyst(void);

/// @brief Opens the LYST wizard, providing a TUI for the LYST settings.
/// @return `true` if the LYST file was successfully written; `false` if the file could not be opened.
bool runInitWizard(void);

#endif
