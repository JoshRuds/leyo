#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/errors.h"

const char *tokenTypeName(TokenType t) {
    switch (t) {
        case STRING: return "STRING";
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case OPENBRAC: return "OPENBRAC";
        case CLOSEBRAC: return "CLOSEBRAC";
        case OPENBRACE: return "OPENBRACE";
        case CLOSEBRACE: return "CLOSEBRACE";
        case OPENSQUARE: return "OPENSQUARE";
        case CLOSESQUARE: return "CLOSESQUARE";
        case COMMA: return "COMMA";
        case SEMICOLON: return "SEMICOLON";
        case OPERATION: return "OPERATION";
        case CONDITION: return "CONDITION";
        case EQUALS: return "EQUALS";
        case UNKNOWN: return "UNKNOWN";
        case ENDOFSTREAM: return "ENDOFSTREAM";
        default: return "???";
    }
}

void printTokenStream(TokenStream ts) {
    logController("Printing token stream");

    for (int i = 0; i < ts.count; i++) {
        Token t = ts.stream[i];

        printf("Token %d:\n", i);
        printf("  Type   : %s\n", tokenTypeName(t.type));
        printf("  Value  : %s\n", t.value);
        printf("  Line   : %d\n", t.line);
        printf("  Column : %d\n", t.collumn);
        printf("\n");
    }

    logController("Finished printing token stream");
}

void printByteCode(ByteCodeResult* bc) {
    logController("Printing bytecode output");

    for (int i = 0; i < bc->length; i++) {
        printf("%02x ", bc->data[i]);
    }
    printf("\n");

    logController("Finished printing bytecode output");
}

char *getVersion() {
    logController("Fetching Version...");
    FILE *vfile = fopen("VERSION.txt", "r");
    if (!vfile) {
        logController("Failed to open input file");
        raise("File open error", 0, 0);
        callAllErr();
    }
    logController("Version Fetched.");
    static char buff[64];
    fgets(buff, 64, vfile);
    return buff;
}

int build(char *filename) {
    logController("Build started");

    FILE *file = fopen(filename, "rb");
    if (!file) {
        logController("Failed to open input file");
        raise("File open error", 0, 0);
        return 1;
    }

    logController("Input file opened successfully");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    
    char *buffer = malloc(size + 1);
    if (!buffer) {
        logController("Memory allocation failed for file buffer");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);

    logController("File loaded into memory");

    TokenStream ts = tokenise(buffer);
    logController("Tokenisation completed");

    printTokenStream(ts);

    if (isErr) {
        logController("Errors detected after tokenisation");
        callAllErr();
    }

    ByteCodeResult bcr = parseToByteCode(ts);
    logController("Parsing to bytecode completed");

    if (!(bcr.data == NULL || bcr.length == 0)) {
        logController("Bytecode successfully generated");
    } else {
        logController("Bytecode generation failed or empty");
    }

    printByteCode(&bcr);

    if (isErr) {
        logController("Errors detected after parsing stage");
        callAllErr();
    }

    logController("Program built successfully");

    return 0;
}

int run() {
    
}

int main(int argc, char *argv[]) {
    initLog("test.log");
    logController("Logger initialized");
    
    const char *version = getVersion();

    logController("Running Leyo");
    logController("Version:");
    logController(version);


    if (argc == 1) { //no cl-arg
        printf("Leyo version v%s\nAuthored by Josh Ruddick", version);
        return 0;
    }

    if (strcmp(argv[1], "build") == 0) {
        if (argc != 3) {
            logController("Too little command line args");
            raise("Too little command line args", 0,0);
            callAllErr();
        }
        return build(argv[2]);
    } else if (strcmp(argv[1], "run") == 0) {
        run();
    } else if (strcmp(argv[1], "repl") == 0) {
        ;
    } else if (strcmp(argv[1], "test") == 0) {
        ;
    } else {
        logController("Unkown command line argument");
        raise("Unkown command line argument", 0,0);
        callAllErr();
    }


    



    return 0;
}