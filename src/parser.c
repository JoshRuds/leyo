#include "../include/type.h"
#include "../include/errors.h"
#include "../include/nodes.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG true
#define LOG(fmt, ...) \
    if (DEBUG) printf("[LOG] " fmt "\n", ##__VA_ARGS__)

static ASTNode *parseExpression(int minPrec);
static ASTNode *parseAtom();

typedef struct {
    Token *tokens;
    int pos;
    int count;
} Parser;

char *VARDEFS[] = {
    "str",
    "int",
    "flt",
};
int VARDEFCOUNT = 3;



Parser parser;
Parser *p;
Program program = {0};


static Token current() {
    LOG("current");
    return p->tokens[p->pos];
}

static Token peek() {
    LOG("peek");
    return p->tokens[p->pos + 1];
}

static void advance() {
    LOG("advnce");
    p->pos++;
    LOG("PROGRAM LOOP pos=%d token=%s", p->pos, current().value);
}

static void expect(TokenType type, char *errorStr) {
    LOG("except");
    if (type != peek().type) {
        raise(errorStr, peek().line, peek().collumn);
    }
    advance();
}

static bool strIsVarDef(char *str) {
    for (int i = 0; i < VARDEFCOUNT; i++) {
        if (strcmp(VARDEFS[i], str) == 0) {
            return true;
        }
    }
    return false;
}


static int precedence(Token tok) {
    if (tok.type != OPERATION) return -1;

    if (strcmp(tok.value, "+") == 0) return 1;
    if (strcmp(tok.value, "-") == 0) return 1;
    if (strcmp(tok.value, "*") == 0) return 2;
    if (strcmp(tok.value, "/") == 0) return 2;

    return -1;
}


static ASTNode *parseAtom() { // smallest possible node
    LOG("parse atom");
    Token tok = current();
    ASTNode *node = malloc(sizeof(ASTNode));

    switch (tok.type) {
        case NUMBER:
            node->type = AST_NUMBER;
            node->value = tok.value;
            advance();
            return node;

        case IDENTIFIER:
            node->type = AST_IDENTIFIER;
            node->value = tok.value;
            advance();
            return node;

        case OPENBRAC: // '('
            advance();

            ASTNode *expr = parseExpression(0);

            if (current().type != CLOSEBRAC) {
                raise("Expected ')'", tok.line, tok.collumn);
                exit(1);
            }

            advance();
            return expr;

        default:
            raise("Expected expression", tok.line, tok.collumn);
            exit(1);
    }
}

static ASTNode *parseExpression(int minPrec) {
    LOG("parse expr");
    ASTNode *left = parseAtom();

    while (true) {
        Token op = current();
        int prec = precedence(op);

        if (prec < minPrec)
            break;

        advance();

        ASTNode *right = parseAtom();

        Token next = current();
        int nextPrec = precedence(next);

        if (nextPrec > prec) {
            right = parseExpression(prec + 1);
        }

        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_BINARY_EXPR;
        node->left = left;
        node->right = right;
        node->value = op.value;

        left = node;
    }

    return left;
}



static void pushProgram(Statement node) {
    if (program.count >= program.capacity) {
        program.capacity = program.capacity == 0 ? 8 : program.capacity * 2;
        program.body = realloc(program.body, sizeof(ASTNode) * program.capacity);
        
        if (!program.body) {
            raise("Memory allocation failed", current().line, current().collumn);
            exit(1);
        }
    }

    program.body[program.count++] = node;
}

static void pushStatement(Statement *stmt, ASTNode node) {
    if (stmt->capacity == 0) {
        stmt->capacity = 8;
        stmt->body = malloc(sizeof(ASTNode) * stmt->capacity);
        if (!stmt->body) {
            raise("Memory allocation failed", 0, 0);
            exit(1);
        }
    }

    if (stmt->count >= stmt->capacity) {
        LOG("REALLOC: count=%d capacity=%d", stmt->count, stmt->capacity);
        stmt->capacity *= 2;
        LOG("REALLOC pt2: count=%d capacity=%d", stmt->count, stmt->capacity);
        stmt->body = realloc(stmt->body, sizeof(ASTNode) * stmt->capacity);

        if (!stmt->body) {
            raise("Memory allocation failed", 0, 0);
            exit(1);
        }
    }

    stmt->body[stmt->count++] = node;
}

static ASTNode astnode(ASTNodeType type, char *value) {
    LOG("Node made");
    ASTNode vardef;
    vardef.type = type;
    vardef.value = value;
    return vardef;
}


void parseVarDef(Statement *stmt) {
    LOG("Var DECL");
    stmt->type = STMT_VAR_DECL;
    pushStatement(stmt, astnode(AST_VAR_DECL, current().value)); // str / flt / int
    expect(IDENTIFIER, "Invalid Sequence For Variable Declaration");
    pushStatement(stmt, astnode(AST_IDENTIFIER, current().value)); // var name
    expect(EQUALS, "Invalid Sequence For Variable Declaration");
    ASTNode *expr = parseExpression(0);
    pushStatement(stmt, *expr);
    expect(SEMICOLON, "Did not find semicolon after statement.");
    advance();
    return;
}


void parseIdentifier(Statement *stmt) {
    LOG("Ident");
    if (strIsVarDef(current().value)) {
        parseVarDef(stmt);
        return;
    } else {
        raise("Unkown Identifier In Body", current().line, current().collumn);
    }
}


Statement parseStatement() {
    LOG("STMT");
    Statement stmt = {0};
    stmt.type = STMT_NULL;
    stmt.count = 0;
    stmt.capacity = 1;
    stmt.startl = current().line;
    stmt.startc = current().collumn;
    stmt.endl = current().line;
    stmt.endc = current().collumn;

    switch (current().type) {
        case IDENTIFIER:
            parseIdentifier(&stmt);
            break;
        
        default:
            raise("Unknown Token Type", current().line, current().collumn);
            break;
    }

    stmt.endl = current().line;
    stmt.endc = current().collumn;
    return stmt;
}

Program parse(TokenStream tokens) {
    LOG("PROGRAM");
    p = &parser;
    p->tokens = tokens.stream;
    p->count = tokens.count;
    p->pos = 0;
    
    while (current().type != ENDOFSTREAM) {
        LOG("PROGRAM LOOP pos=%d token=%s", p->pos, current().value);
        pushProgram(parseStatement());
    }

    return program;
}