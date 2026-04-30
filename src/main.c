#include <stdio.h>
#include <stdlib.h>
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
    for (int i = 0; i < ts.count; i++) {
        Token t = ts.stream[i];

        printf("Token %d:\n", i);
        printf("  Type   : %s\n", tokenTypeName(t.type));
        printf("  Value  : %s\n", t.value);
        printf("  Line   : %d\n", t.line);
        printf("  Column : %d\n", t.collumn);
        printf("\n");
    }
}

static void printIndent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

void printAST(ASTNode *node, int depth) {
    if (!node) return;

    printIndent(depth);

    switch (node->type) {

        case AST_NUMBER:
            printf("NUMBER: %s\n", node->value);
            break;

        case AST_IDENTIFIER:
            printf("IDENTIFIER: %s\n", node->value);
            break;

        case AST_BINARY_EXPR:
            printf("BINARY_EXPR: %s\n", node->value ? node->value : "op");

            printAST(node->left, depth + 1);
            printAST(node->right, depth + 1);
            break;

        default:
            printf("UNKNOWN AST NODE\n");
            break;
    }

    // Optional: print children (only if you actually use them)
    if (node->children && node->childCount > 0) {
        printIndent(depth);
        printf("CHILDREN:\n");

        for (int i = 0; i < node->childCount; i++) {
            printAST(&node->children[i], depth + 1);
        }
    }
}

void printStatement(Statement *stmt, int depth) {
    printIndent(depth);

    switch (stmt->type) {

        case STMT_VAR_DECL:
            printf("VAR_DECL\n");

            for (int i = 0; i < stmt->count; i++) {
                printAST(&stmt->body[i], depth + 1);
            }
            break;

        case STMT_BLOCK:
            printf("BLOCK\n");
            for (int i = 0; i < stmt->count; i++) {
                printAST(&stmt->body[i], depth + 1);
            }
            break;

        case STMT_EMPTY:
            printf("EMPTY\n");
            break;

        default:
            printf("UNKNOWN STATEMENT\n");
            break;
    }
}

void printProgram(Program *program) {
    printf("PROGRAM\n");

    for (int i = 0; i < program->count; i++) {
        printStatement(&program->body[i], 1);
    }
}

int main() {

    FILE *file = fopen("test.leyo", "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *buffer = malloc(size + 1);
    if (!buffer) return 1;
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);


    TokenStream ts = tokenise(buffer);
    printTokenStream(ts);

    if (isErr) {
        callAllErr();
    }

    Program ast = parse(ts);
    if (!(ast.body == NULL || ast.count == 0)) {
        printf("AST IS ALIVE!!!\n");
    }
    printProgram(&ast);
    

    if (isErr) {
        callAllErr();
    }

    return 0;
}