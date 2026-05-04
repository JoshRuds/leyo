#include "../include/errors.h"
#include "../include/parser.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t *data;
    int dataCount;
    int pos;
    int nibblePos; // 0 = high nibble, 1 = low nibble
} VM;

VM vm;
VM *v;

static uint8_t currentByte() {
    if (v->pos >= v->dataCount) return 0;
    return v->data[v->pos];
}

static uint8_t peekByte() {
    if (v->pos + 1 >= v->dataCount) return 0;
    return v->data[v->pos + 1];
}

static uint8_t advanceByte() {
    if (v->pos >= v->dataCount) return 0;
    return v->data[v->pos++];
}

static uint8_t currentNibble() {
    if (v->pos >= v->dataCount) return 0;

    uint8_t byte = v->data[v->pos];
    return (v->nibblePos == 0) ? ((byte >> 4) & 0xF) : (byte & 0xF);
}

static uint8_t peekNibble() {
    if (v->pos >= v->dataCount) return 0;

    if (v->nibblePos == 0) {
        return v->data[v->pos] & 0xF;
    } else {
        if (v->pos + 1 >= v->dataCount) return 0;
        return (v->data[v->pos + 1] >> 4) & 0xF;
    }
}

static uint8_t peekNibble() {
    if (v->pos >= v->dataCount) return 0;

    if (v->nibblePos == 0) {
        return v->data[v->pos] & 0xF;
    } else {
        if (v->pos + 1 >= v->dataCount) return 0;
        return (v->data[v->pos + 1] >> 4) & 0xF;
    }
}

static void expectCurrent(uint8_t type, char *errorStr) {
    if (type != currentByte()) {
        logBuildParser("Expect failed (current mismatch)");
        raise(errorStr, 0,0);
    }
    advance();
}

static void expect(uint8_t type, char *errorStr) {
    if (type != peekByte()) {
        logBuildParser("Expect failed (peek mismatch)");
        raise(errorStr, 0,0);
    }
    advance();
}

static void expectAndPass(uint8_t type, char *errorStr) {
    if (type != peekByte()) {
        logBuildParser("ExpectAndPass failed");
        raise(errorStr, 0,0);
    }
    advance();
    advance();
}



// takes in bytecode and runs it 
int runByteCode(ByteCodeResult bcr) {
    vm.data = bcr.data;
    vm.dataCount = bcr.length;
    vm.pos = 0;
    vm.nibblePos = 0;

    v = &vm;
    logRuntime("VM STARTED");
    bcr.length++;
    return 0;
}