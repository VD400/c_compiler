#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef enum {
    NODE_INT, NODE_FLOAT, NODE_ID, NODE_VAR_DECL,
    NODE_BIN_OP, NODE_ASSIGN, NODE_IF, NODE_WHILE,
    NODE_PRINT, NODE_STMT_LST, NODE_RETURN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* value;               
    char* data_type;           // ADDED: To store "int" or "float" for declarations
    struct ASTNode *left;      
    struct ASTNode *right;     
    struct ASTNode *condition; 
    struct ASTNode *else_body; 
    struct ASTNode *next;      
} ASTNode;

// Constructor Functions
ASTNode* create_node(NodeType type, char* value, ASTNode* left, ASTNode* right);
ASTNode* create_if_node(ASTNode* cond, ASTNode* body, ASTNode* else_b);
ASTNode* create_while_node(ASTNode* cond, ASTNode* body);
// ADDED: Helper for variable declarations
ASTNode* create_var_node(char* data_type, char* var_name);

#endif