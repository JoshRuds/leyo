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

/// @brief 
/// @param  
/// @return 
LogConfig readLogConfig(void);

/// @brief Writes the default LYST
/// @param  
/// @return 
bool writeDefaultLyst(void);
bool runInitWizard(void);

#endif
