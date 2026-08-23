/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file repl.c
/// @brief The REPL.

#include "lexer.h"
#include "parser.h"
#include "errors.h"
#include "vm.h"
#include <stdio.h>
#include <string.h>

int repl(void) {
    logController("REPL requested");
    lraise(WF_GENERAL, ERR_NOT_IMPLEMENTED_YET, 0,0, NULL);
    return 1;
    TokenStream tokens;
    ByteCodeResult res;
    while (1) {
        printf(">>> ");
        char *buff = "";
        scanf("%s", buff);
        
        tokens = tokenise(buff);
        if (strcmp(tokens.stream[0].value, "exit") == 0) {return 0;};

        res = parse(&tokens, "REPL");

        runVM(res, false, "REPL");
    }
}
