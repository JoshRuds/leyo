/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file external.c
/// @brief External network functionality.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char githubLink[] = "https://github.com/leyo-lang/leyo";

void gotoLink(const char *link) {
    size_t maxStrSize = ((strlen(link) + 10) * sizeof(char));
    char *command = malloc(maxStrSize);

#ifdef _WIN32
    snprintf(command, maxStrSize, "start %s", link);
#elif __APPLE__
    snprintf(command, maxStrSize, "open %s", link);
#else
    snprintf(command, maxStrSize, "xdg-open %s", link);
#endif

    system(command);
}