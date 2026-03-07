#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef enum {
    NODE_INT,
    NODE_FLOAT, 
    NODE_CHAR,
    NODE_STR,
    NODE_ID, 
    NODE_VAR_DECL,
    NODE_BIN_OP, 
    NODE_ASSIGN, 
    NODE_IF, 
    NODE_WHILE,
    NODE_PRINT, 
    NODE_SCAN,
    NODE_STMT_LST, 
    NODE_RETURN,
    NODE_BREAK,
    NODE_CONTINUE,
    NODE_BLOCK,      // For { statement_list }
    NODE_REL_OP,     // For <, >, <=, >=, ==, !=
    NODE_LOGIC_OP    // For &&, ||
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
ASTNode* create_io_node(NodeType type, ASTNode* target);
ASTNode* create_jump_node(NodeType type, ASTNode* return_value);
ASTNode* create_block_node(ASTNode* stmt_list);
void print_ast(ASTNode* node, int level);


#endif