/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef EXTERNAL_H
#define EXTERNAL_H

/// @brief The link to the leyo github repository.
extern const char githubLink[];

/// @brief Opens default browser to the given link.
/// @param link The link to go to.
void gotoLink(const char *link);

#endif