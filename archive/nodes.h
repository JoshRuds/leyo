#ifndef NODE_H
#define NODE_H


typedef enum {
    AST_NUMBER,
    AST_IDENTIFIER,
    AST_EXPRESSION,
    AST_BINARY_EXPR,
    AST_VAR_DECL,
} ASTNodeType;

typedef enum {
    STMT_VAR_DECL,
    STMT_BLOCK,
    STMT_EMPTY,
    STMT_NULL
} ASTStmtType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    ASTNodeType type;
    ASTNode *left;
    ASTNode *right;
    char *value;
    ASTNode *children;
    int childCount;
};

typedef struct {
    ASTStmtType type;
    ASTNode *body;
    int count;
    int capacity;
    int startl;
    int startc;
    int endl;
    int endc;
} Statement;

typedef struct {
    int capacity;
    int count;
    Statement *body;
} Program;

#endif