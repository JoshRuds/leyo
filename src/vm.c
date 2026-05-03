#include "../include/errors.h"
#include "../include/parser.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t data;
    int dataCount;
    int pos;
    
} VM;

VM vm;
VM *v;

// takes in bytecode and runs it 
int runByteCode(ByteCodeResult bcr) {
    logRuntime("VM STARTED");
    bcr.length++;
    return 0;
}