#include "../include/type.h"
#include "../include/errors.h"
#include "../include/nodes.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    Token *tokens;
    int pos;
    int count;
    uint8_t bytebuff[2048];
    int byteIndex;
    int hasHalfByte;
    uint8_t halfByte;
} ByteCoder;

typedef struct {
    uint8_t *data;
    int length;
} ByteCodeResult;

static char *VARDEFS[] = {
    "str",
    "int",
    "flt",
};
static int VARDEFCOUNT = 3;

ByteCoder bytecoder = {0};
ByteCoder *b;



static Token current() {
    return b->tokens[b->pos];
}

static Token peek() {
    return b->tokens[b->pos+1];
}

static void advance() {
    b->pos++;
}

static void expect(TokenType type, char *errorStr) {
    if (type != peek().type) {
        raise(errorStr, peek().line, peek().collumn);
    }
    advance();
}

static void expectAndPass(TokenType type, char *errorStr) {
    if (type != peek().type) {
        raise(errorStr, peek().line, peek().collumn);
    }
    advance();
    advance();
}

static void writeByte(uint8_t value) {
    if ((size_t)b->byteIndex >= sizeof(b->bytebuff)) {
        raise("Byte buffer overflow", current().line, current().collumn);
    }

    b->bytebuff[b->byteIndex] = value;
    b->byteIndex++;
}

static void writeHalfByte(uint8_t value) {
    if (!b->hasHalfByte) {
        b->halfByte = value & 0xF;
        b->hasHalfByte = 1;
    } else {
        uint8_t full = (b->halfByte << 4) | (value & 0xF);
        writeByte(full);
        b->hasHalfByte = 0;
    }
}

static void writeRawExpr(char *expr, uint8_t opcode) {
    uint8_t len = (uint8_t)strlen(expr);

    writeByte(opcode);

    for (uint8_t i = 0; i < len; i++) {
        writeByte((uint8_t)expr[i]);
    }
    writeByte(0xFF);
}

static bool strInVarDef(char *str) {
    for (int i = 0; i < VARDEFCOUNT; i++) {
        if (strcmp(VARDEFS[i], str) == 0) {
            return true;
        }
    }
    return false;
}

static void parseVarDecl() {
    writeHalfByte(0x1);
    int arrOff = 0;
    if (strcmp(peek().value, "arr") == 0) {
        arrOff = 1;
    }

    if (strcmp(current().value, "int") == 0) {
        writeHalfByte(0x1 + arrOff);
    } else {
        raise("Internal Parser Error - Dead Var Decl",
            current().line,
            current().collumn);
    }

    if (arrOff) {
        advance(); // int arr *x*
    }
    expect(IDENTIFIER, "No identifier after var decl");
    writeRawExpr(current().value, 0xFE);
    expectAndPass(EQUALS, "No equals after var decl identifier");

    char expr[1024];
    int exprLoc = 0;

    while (current().type != SEMICOLON) {
        char *val = current().value;
        for (int j = 0; val[j] != '\0'; j++) {
            expr[exprLoc++] = val[j];
        }
        advance();
    }

    expr[exprLoc] = '\0';
    writeRawExpr(expr, 0xFD);

    advance(); // past semicolon
}

static void parseStatement() {
    switch (current().type) {
        case IDENTIFIER:
            if (strInVarDef(current().value)) {
                parseVarDecl();
            }
            break;
        
        default:
            raise("Unknown Token Type", current().line, current().collumn);
            break;
    }
    writeByte(0xf0); // end of line
}

ByteCodeResult parseToByteCode(TokenStream ts) {
    b = &bytecoder;
    b->tokens = ts.stream;
    b->pos = 0;
    b->byteIndex = 0;
    b->hasHalfByte = 0;

    while (current().type != ENDOFSTREAM) {
        parseStatement();
    }
    writeByte(0x00);

    if (b->hasHalfByte) {
        raise("Unpaired half-byte", current().line, current().collumn);
        callAllErr();
    }
    ByteCodeResult res = {0};
    res.data = b->bytebuff;
    res.length = b->byteIndex;
    return res;
}