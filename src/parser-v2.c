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

char *VARDEFS[] = {
    "str",
    "int",
    "flt",
};
int VARDEFCOUNT = 3;

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
    if (b->byteIndex >= sizeof(b->bytebuff)) {
        raise("Byte buffer overflow", current().line, current().collumn);
    }

    b->bytebuff[b->byteIndex] = value;
    b->byteIndex++;
}

static void writeHexValue(uint8_t value, int digits) {
    if (!b->hasHalfByte) {
        b->halfByte = value & 0xF;
        b->hasHalfByte = 1;
    } else {
        uint8_t full = (b->halfByte << 4) | (value & 0xF);
        writeByte(full);
        b->hasHalfByte = 0;
    }
}

static bool strInVarDef(char *str) {
    for (int i = 0; i < VARDEFCOUNT; i++) {
        if (strcmp(VARDEFS[i], str) == 0) {
            return true;
        }
    }
    return false;
}

void parseStatement() {

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



    if (b->hasHalfByte) {
        raise("Unpaired half-byte", current().line, current().collumn);
        callAllErr();
    }
    ByteCodeResult res = {0};
    res.data = b->bytebuff;
    res.length = b->byteIndex;
    return res;
}