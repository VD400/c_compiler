#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Bring in the global file pointer from main.c
extern FILE* ast_file;

#define PRINT_AST(...) do { fprintf(stdout, __VA_ARGS__); fprintf(ast_file, __VA_ARGS__); } while(0)

// The basic "Lego Brick" creator
ASTNode* create_node(NodeType type, char* value, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if(!node){
        fprintf(stderr,"Error: Memory allocation failed in create_node\n");
        exit(1);
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

ASTNode* create_block_node(ASTNode* stmt_list) {
    // We use the 'left' pointer to point to the start of the statement list
    ASTNode* node = create_node(NODE_BLOCK, NULL, stmt_list, NULL);
    return node;
}

// Helper function to handle the formatting and recursive depth
void print_ast_node(ASTNode* node, const char* prefix, int is_last) {
    if (!node) return;

    // 1. Print the branch connector
/*    fprintf(ast_file, "%s%s", prefix, is_last ? "└── " : "├── ");*/
    PRINT_AST("%s%s", prefix, is_last ? "└── " : "├── ");

    // 2. Prepare the prefix for the next generation of children
    char new_prefix[1024];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");

    // 3. Print the node label and recurse to its children
    switch (node->type) {
        case NODE_VAR_DECL:
/*            fprintf(ast_file, "DECLARATION %s %s\n", node->data_type ? node->data_type : "unknown", node->value);*/
            PRINT_AST("DECLARATION %s %s\n", node->data_type ? node->data_type : "unknown", node->value);
            break;
            
        case NODE_ASSIGN:
/*            fprintf(ast_file, "ASSIGN\n");*/
            PRINT_AST("ASSIGN\n");
            print_ast_node(node->left, new_prefix, 0); // x
            print_ast_node(node->right, new_prefix, 1); // 10
            break;
            
        case NODE_BIN_OP:
            PRINT_AST("(%s)\n", node->value);
/*            fprintf(ast_file, "(%s)\n", node->value);*/
            print_ast_node(node->left, new_prefix, 0);
            print_ast_node(node->right, new_prefix, 1);
            break;
            
        case NODE_ID:
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_STR:
/*            fprintf(ast_file, "%s\n", node->value ? node->value : "");*/
         PRINT_AST("%s\n", node->value ? node->value : "");
            break;
            
        case NODE_PRINT:
            PRINT_AST("PRINT\n");
            print_ast_node(node->left, new_prefix, 1);
            break;
            
        case NODE_SCAN:
            PRINT_AST("SCAN\n");
            print_ast_node(node->left, new_prefix, 1);
            break;
            
        case NODE_RETURN:
            PRINT_AST("RETURN\n");
            if (node->left) print_ast_node(node->left, new_prefix, 1);
            break;
            
        case NODE_BREAK:
            PRINT_AST("BREAK\n");
            break;
            
        case NODE_CONTINUE:
            PRINT_AST("CONTINUE\n");
            break;
            
        case NODE_IF:
            PRINT_AST("IF\n");
            // Condition block
            PRINT_AST("%s├── CONDITION\n", new_prefix);
            char cond_prefix[2048];
            snprintf(cond_prefix, sizeof(cond_prefix), "%s│   ", new_prefix);
            print_ast_node(node->condition, cond_prefix, 1);
            
            // True Body block
            int has_else = (node->else_body != NULL);
            if (node->left) {
                ASTNode* curr = node->left;
                while (curr) {
                    // It's only the "last" statement if it's the end of the list AND there's no ELSE block
                    int is_last_stmt = (curr->next == NULL) && !has_else;
                    print_ast_node(curr, new_prefix, is_last_stmt);
                    curr = curr->next;
                }
            }
            
            // Else Body block
            if (has_else) {
                PRINT_AST("%s└── ELSE\n", new_prefix);
                char else_prefix[2048];
                snprintf(else_prefix, sizeof(else_prefix), "%s    ", new_prefix);
                
                ASTNode* curr = node->else_body;
                while (curr) {
                    int is_last_stmt = (curr->next == NULL);
                    print_ast_node(curr, else_prefix, is_last_stmt);
                    curr = curr->next;
                }
            }
            break;
            
        case NODE_WHILE:
            PRINT_AST("WHILE\n");
            // Condition block
            PRINT_AST("%s├── CONDITION\n", new_prefix);
            char wcond_prefix[2048];
            snprintf(wcond_prefix, sizeof(wcond_prefix), "%s│   ", new_prefix);
            print_ast_node(node->condition, wcond_prefix, 1);

            // Body block
            if (node->left) {
                ASTNode* curr = node->left;
                while (curr) {
                    int is_last_stmt = (curr->next == NULL);
                    print_ast_node(curr, new_prefix, is_last_stmt);
                    curr = curr->next;
                }
            }
            break;
            
        default:
            PRINT_AST("UNKNOWN_NODE_TYPE (%d)\n", node->type);
            break;
    }
}

// Wrapper function to start the printing process
void print_ast(ASTNode* node, int level) {
    if (!node) return;
    
    // Always print PROGRAM as the root of the tree
    PRINT_AST("PROGRAM\n");
    
    // In your parser, statements are chained using the 'next' pointer
    ASTNode* curr = node;
    while (curr) {
        // Evaluate if this statement is the final one in the entire program
        int is_last = (curr->next == NULL);
        
        // Start recursive printing with an empty prefix
        print_ast_node(curr, "", is_last);
        
        curr = curr->next;
    }
}

/*#include<stdio.h>*/
/*#include <stdlib.h>*/
/*#include <string.h>*/
/*#include "ast.h"*/


/*// The basic "Lego Brick" creator*/
/*ASTNode* create_node(NodeType type, char* value, ASTNode* left, ASTNode* right) {*/
/*    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));*/
/*    if(!node){*/
/*        fprintf(stderr,"Error: Memory allocation failed in create_node\n");*/
/*        exit(1);*/
/*    }*/
/*    node->type = type;*/
/*    node->value = value ? strdup(value) : NULL;*/
/*    node->data_type = NULL;*/
/*    node->left = left;*/
/*    node->right = right;*/
/*    node->condition = NULL;*/
/*    node->else_body = NULL;*/
/*    node->next = NULL;*/
/*    return node;*/
/*}*/

/*// Specialized creator for IF*/
/*ASTNode* create_if_node(ASTNode* cond, ASTNode* body, ASTNode* else_b) {*/
/*    ASTNode* node = create_node(NODE_IF, NULL, body, NULL);*/
/*    node->condition = cond;*/
/*    node->else_body = else_b;*/
/*    return node;*/
/*}*/

/*// Specialized creator for WHILE*/
/*ASTNode* create_while_node(ASTNode* cond, ASTNode* body) {*/
/*    ASTNode* node = create_node(NODE_WHILE, NULL, body, NULL);*/
/*    node->condition = cond;*/
/*    return node;*/
/*}*/

/*ASTNode* create_io_node(NodeType type, ASTNode* target){*/
/*    return create_node(type, NULL, target, NULL);*/
/*}*/

/*// Specialized creator for Variables (e.g., int x)*/
/*ASTNode* create_var_node(char* data_type, char* var_name) {*/
/*    ASTNode* node = create_node(NODE_VAR_DECL, var_name, NULL, NULL);*/
/*    node->data_type = strdup(data_type);*/
/*    return node;*/
/*}*/

/*ASTNode* create_jump_node(NodeType type, ASTNode* return_value){*/
/*    ASTNode* node = create_node(type, NULL, return_value, NULL);*/
/*    return node;*/
/*}*/

/*ASTNode* create_block_node(ASTNode* stmt_list) {*/
/*    // We use the 'left' pointer to point to the start of the statement list*/
/*    ASTNode* node = create_node(NODE_BLOCK, NULL, stmt_list, NULL);*/
/*    return node;*/
/*}*/

/*const char* node_type_to_string(NodeType type){*/
/*    switch(type){*/
/*        case NODE_INT: return "INT";*/
/*        case NODE_FLOAT: return "FLOAT";*/
/*        case NODE_CHAR: return "CHAR";*/
/*        case NODE_STR: return "STRING";*/
/*        case NODE_ID: return "IDENTIFIER";*/
/*        case NODE_VAR_DECL: return "VAR_DECL";*/
/*        case NODE_BIN_OP: return "BINARY_OP";*/
/*        case NODE_ASSIGN: return "ASSIGN";*/
/*        case NODE_IF: return "IF";*/
/*        case NODE_WHILE: return "WHILE";*/
/*        case NODE_PRINT: return "PRINT";*/
/*        case NODE_SCAN: return "SCAN";*/
/*        case NODE_STMT_LST: return "STMT_LIST";*/
/*        case NODE_RETURN: return "RETURN";*/
/*        case NODE_BREAK: return "BREAK";*/
/*        case NODE_CONTINUE: return "CONTINUE";*/
/*        default: return "UNKNOWN";*/
/*    }*/
/*}*/



/*void print_ast(ASTNode* node, int level) {*/
/*    if (node == NULL) return;*/

/*    // Indent for a tree-like look*/
/*    for (int i = 0; i < level; i++) printf("  ");*/

/*    // Print the type and value*/
/*    printf("|-- Type: %d, Value: %s\n", node->type, node->value ? node->value : "NULL");*/

/*    // Recursively print children*/
/*    print_ast(node->left, level + 1);*/
/*    print_ast(node->right, level + 1);*/
/*    */
/*    // If it's an IF or WHILE, print the condition separately*/
/*    if (node->condition) {*/
/*        for (int i = 0; i < level; i++) printf("  ");*/
/*        printf("  [Condition]:\n");*/
/*        print_ast(node->condition, level + 2);*/
/*    }*/

/*    if(node->else_body){*/
/*        for(int i = 0; i < level; i++){*/
/*            printf("  ");*/
/*        }*/
/*        printf("  [Else Body]:\n");*/
/*        print_ast(node->else_body, level+2);*/
/*    }*/

/*    // Move to the next statement in the block*/
/*    if (node->next) print_ast(node->next, level);*/
/*}*/

/*// Helper function to handle the formatting and recursive depth*/
/*void print_ast_node(ASTNode* node, const char* prefix, int is_last) {*/
/*    if (!node) return;*/

/*    // 1. Print the branch connector*/
/*    printf("%s%s", prefix, is_last ? "└── " : "├── ");*/

/*    // 2. Prepare the prefix for the next generation of children*/
/*    char new_prefix[1024];*/
/*    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");*/

/*    // 3. Print the node label and recurse to its children*/
/*    switch (node->type) {*/
/*        case NODE_VAR_DECL:*/
/*            printf("DECLARATION %s %s\n", node->data_type ? node->data_type : "unknown", node->value);*/
/*            break;*/
/*            */
/*        case NODE_ASSIGN:*/
/*            printf("ASSIGN\n");*/
/*            print_ast_node(node->left, new_prefix, 0); // x*/
/*            print_ast_node(node->right, new_prefix, 1); // 10*/
/*            break;*/
/*            */
/*        case NODE_BIN_OP:*/
/*            printf("(%s)\n", node->value);*/
/*            print_ast_node(node->left, new_prefix, 0);*/
/*            print_ast_node(node->right, new_prefix, 1);*/
/*            break;*/
/*            */
/*        case NODE_ID:*/
/*        case NODE_INT:*/
/*        case NODE_FLOAT:*/
/*        case NODE_CHAR:*/
/*        case NODE_STR:*/
/*            printf("%s\n", node->value ? node->value : "");*/
/*            break;*/
/*            */
/*        case NODE_PRINT:*/
/*            printf("PRINT\n");*/
/*            print_ast_node(node->left, new_prefix, 1);*/
/*            break;*/
/*            */
/*        case NODE_SCAN:*/
/*            printf("SCAN\n");*/
/*            print_ast_node(node->left, new_prefix, 1);*/
/*            break;*/
/*            */
/*        case NODE_RETURN:*/
/*            printf("RETURN\n");*/
/*            if (node->left) print_ast_node(node->left, new_prefix, 1);*/
/*            break;*/
/*            */
/*        case NODE_BREAK:*/
/*            printf("BREAK\n");*/
/*            break;*/
/*            */
/*        case NODE_CONTINUE:*/
/*            printf("CONTINUE\n");*/
/*            break;*/
/*            */
/*        case NODE_IF:*/
/*            printf("IF\n");*/
/*            // Condition block*/
/*            printf("%s├── CONDITION\n", new_prefix);*/
/*            char cond_prefix[2048];*/
/*            snprintf(cond_prefix, sizeof(cond_prefix), "%s│   ", new_prefix);*/
/*            print_ast_node(node->condition, cond_prefix, 1);*/
/*            */
/*            // True Body block*/
/*            int has_else = (node->else_body != NULL);*/
/*            if (node->left) {*/
/*                ASTNode* curr = node->left;*/
/*                while (curr) {*/
/*                    // It's only the "last" statement if it's the end of the list AND there's no ELSE block*/
/*                    int is_last_stmt = (curr->next == NULL) && !has_else;*/
/*                    print_ast_node(curr, new_prefix, is_last_stmt);*/
/*                    curr = curr->next;*/
/*                }*/
/*            }*/
/*            */
/*            // Else Body block*/
/*            if (has_else) {*/
/*                printf("%s└── ELSE\n", new_prefix);*/
/*                char else_prefix[2048];*/
/*                snprintf(else_prefix, sizeof(else_prefix), "%s    ", new_prefix);*/
/*                */
/*                ASTNode* curr = node->else_body;*/
/*                while (curr) {*/
/*                    int is_last_stmt = (curr->next == NULL);*/
/*                    print_ast_node(curr, else_prefix, is_last_stmt);*/
/*                    curr = curr->next;*/
/*                }*/
/*            }*/
/*            break;*/
/*            */
/*        case NODE_WHILE:*/
/*            printf("WHILE\n");*/
/*            // Condition block*/
/*            printf("%s├── CONDITION\n", new_prefix);*/
/*            char wcond_prefix[2048];*/
/*            snprintf(wcond_prefix, sizeof(wcond_prefix), "%s│   ", new_prefix);*/
/*            print_ast_node(node->condition, wcond_prefix, 1);*/

/*            // Body block*/
/*            if (node->left) {*/
/*                ASTNode* curr = node->left;*/
/*                while (curr) {*/
/*                    int is_last_stmt = (curr->next == NULL);*/
/*                    print_ast_node(curr, new_prefix, is_last_stmt);*/
/*                    curr = curr->next;*/
/*                }*/
/*            }*/
/*            break;*/
/*            */
/*        default:*/
/*            printf("UNKNOWN_NODE_TYPE (%d)\n", node->type);*/
/*            break;*/
/*    }*/
/*}*/

/*// Wrapper function to start the printing process*/
/*void print_ast(ASTNode* node, int level) {*/
/*    if (!node) return;*/
/*    */
/*    // Always print PROGRAM as the root of the tree*/
/*    printf("PROGRAM\n");*/
/*    */
/*    // In your parser, statements are chained using the 'next' pointer*/
/*    ASTNode* curr = node;*/
/*    while (curr) {*/
/*        // Evaluate if this statement is the final one in the entire program*/
/*        int is_last = (curr->next == NULL);*/
/*        */
/*        // Start recursive printing with an empty prefix*/
/*        print_ast_node(curr, "", is_last);*/
/*        */
/*        curr = curr->next;*/
/*    }*/
/*}*/


