#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// The basic "Lego Brick" creator
ASTNode* create_node(NodeType type, char* value, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if(!node){
        fprintf(stderr,"Error: Memory allocation failed in create_node\n");
    }
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->data_type = NULL;
    node->left = left;
    node->right = right;
    node->condition = NULL;
    node->else_body = NULL;
    node->next = NULL;
    return node;
}

// Specialized creator for IF
ASTNode* create_if_node(ASTNode* cond, ASTNode* body, ASTNode* else_b) {
    ASTNode* node = create_node(NODE_IF, NULL, body, NULL);
    node->condition = cond;
    node->else_body = else_b;
    return node;
}

// Specialized creator for WHILE
ASTNode* create_while_node(ASTNode* cond, ASTNode* body) {
    ASTNode* node = create_node(NODE_WHILE, NULL, body, NULL);
    node->condition = cond;
    return node;
}

ASTNode* create_io_node(NodeType type, ASTNode* target){
    return create_node(type, NULL, target, NULL);
}

// Specialized creator for Variables (e.g., int x)
ASTNode* create_var_node(char* data_type, char* var_name) {
    ASTNode* node = create_node(NODE_VAR_DECL, var_name, NULL, NULL);
    node->data_type = strdup(data_type);
    return node;
}

ASTNode* create_jump_node(NodeType type, ASTNode* return_value){
    ASTNode* node = create_node(type, NULL, return_value, NULL);
    return node;
}


void print_ast(ASTNode* node, int level) {
    if (node == NULL) return;

    // Indent for a tree-like look
    for (int i = 0; i < level; i++) printf("  ");

    // Print the type and value
    printf("|-- Type: %d, Value: %s\n", node->type, node->value ? node->value : "NULL");

    // Recursively print children
    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
    
    // If it's an IF or WHILE, print the condition separately
    if (node->condition) {
        for (int i = 0; i < level; i++) printf("  ");
        printf("  [Condition]:\n");
        print_ast(node->condition, level + 2);
    }

    if(node->else_body){
        for(int i = 0; i < level; i++){
            printf("  ");
        }
        printf("  [Else Body]:\n");
        print_ast(node->else_body, level+2);
    }

    // Move to the next statement in the block
    if (node->next) print_ast(node->next, level);
}