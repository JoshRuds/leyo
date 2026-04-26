#ifndef NODE_H
#define NODE_H

typedef struct {
    int line;
    int collumn;
} Location;

typedef enum {
    AST_NUMBER,
    AST_IDENTIFIER,
    AST_BINARY_EXPR,
    AST_VAR_DECL
} ASTNodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    ASTNodeType type;
    ASTNode *left;
    ASTNode *right;
    char *value;
};

typedef struct {
    ASTNode *body;
    Location start;
    Location end;
} Statement;

typedef struct {
    Location start;
    Location end;
    Statement *body;
} Program;

#endif