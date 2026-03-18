#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

// Helper function signature
char* analyze_semantics(ASTNode* node);

char* analyze_semantics(ASTNode* node) {
    if (node == NULL) return NULL;

    char* return_type = NULL;

    switch (node->type) {
        // --- 1. DECLARATIONS & VARIABLES ---
        case NODE_VAR_DECL:
            // Add to symbol table and return its type
            add_symbol(node->value, node->data_type);
            return_type = node->data_type;
            break;

        case NODE_ID: {
            // Verify it exists in the symbol table
            Symbol* sym = lookup_symbol(node->value);
            if (sym == NULL) {
                fprintf(stderr, "Semantic Error: Variable '%s' not declared before use.\n", node->value);
                exit(1);
            }
            return_type = sym->data_type;
            break;
        }

        // --- 2. LITERALS (Base cases for recursion) ---
        case NODE_INT:   return_type = "int"; break;
        case NODE_FLOAT: return_type = "float"; break;
        case NODE_CHAR:  return_type = "char"; break;
        case NODE_STR:   return_type = "string"; break;

        // --- 3. ASSIGNMENT & TYPE CHECKING ---
        case NODE_ASSIGN: {
            // Check if the variable on the left exists
            Symbol* sym = lookup_symbol(node->left->value); 
            if (sym == NULL) {
                fprintf(stderr, "Semantic Error: Assignment to undeclared variable '%s'\n", node->left->value);
                exit(1);
            }
            
            // Get the type of the mathematical expression on the right
            char* right_type = analyze_semantics(node->right);
            
            // Perform Type Checking
            if (right_type != NULL && strcmp(sym->data_type, right_type) != 0) {
                fprintf(stderr, "Warning: Type mismatch in assignment. Assigning '%s' to '%s' variable '%s'\n", 
                        right_type, sym->data_type, sym->name);
            }
            return_type = sym->data_type;
            break;
        }

        case NODE_BIN_OP: {
            char* left_type = analyze_semantics(node->left);
            char* right_type = analyze_semantics(node->right);
            
            // Basic type coercion: if either side is a float, the result is a float
            if (left_type && right_type) {
                if (strcmp(left_type, "float") == 0 || strcmp(right_type, "float") == 0) {
                    return_type = "float";
                } else {
                    return_type = "int";
                }
            }
            break;
        }

        // --- 4. SCOPE MANAGEMENT & CONTROL FLOW ---
        case NODE_BLOCK:
            // Create a new scope for the `{ ... }` block
            push_scope();
            analyze_semantics(node->left); // Traverse the statements inside the block
            pop_scope();
            break;

        case NODE_IF:
            analyze_semantics(node->condition);
            analyze_semantics(node->left);      // True block
            if (node->else_body) {
                analyze_semantics(node->else_body); // Else block
            }
            break;

        case NODE_WHILE:
            analyze_semantics(node->condition);
            analyze_semantics(node->left);      // Loop body
            break;

        // --- 5. BUILT-IN FUNCTIONS & JUMPS ---
        case NODE_PRINT:
        case NODE_SCAN:
        case NODE_RETURN:
            // Just check the expressions inside them
            analyze_semantics(node->left);
            break;

        case NODE_BREAK:
        case NODE_CONTINUE:
            // Nothing to check for simple jumps
            break;

        default:
            // Fallback for any unhandled node types
            analyze_semantics(node->left);
            analyze_semantics(node->right);
            break;
    }

    // Traverse to the next statement in the current block
    if (node->next) {
        analyze_semantics(node->next);
    }

    return return_type;
}




/*#include <stdio.h>*/
/*#include <stdlib.h>*/
/*#include "ast.h"*/
/*#include "symbol_table.h"*/

/*void analyze_semantics(ASTNode* node)*/
/*{*/
/*    if(node == NULL)*/
/*        return;*/

/*    switch(node->type)*/
/*    {*/

/*        case NODE_VAR_DECL:*/
/*            add_symbol(node->value, node->data_type);*/
/*            break;*/

/*        case NODE_ID:*/
/*            if(lookup_symbol(node->value) == NULL){*/
/*                fprintf(stderr,"Semantic Error: %s not declared\n",node->value);*/
/*                exit(1);*/
/*            }*/
/*            break;*/

/*        case NODE_ASSIGN:*/
/*            analyze_semantics(node->left);*/
/*            analyze_semantics(node->right);*/
/*            break;*/

/*        case NODE_BLOCK:*/
/*            push_scope();*/
/*            analyze_semantics(node->left);*/
/*            pop_scope();*/
/*            break;*/

/*        case NODE_IF:*/
/*        case NODE_WHILE:*/

/*            analyze_semantics(node->condition);*/
/*            analyze_semantics(node->left);*/
/*            analyze_semantics(node->else_body);*/
/*            break;*/

/*        case NODE_STMT_LST:*/

/*            analyze_semantics(node->left);*/
/*            break;*/

/*        default:*/

/*            analyze_semantics(node->left);*/
/*            analyze_semantics(node->right);*/
/*            break;*/
/*    }*/

/*    analyze_semantics(node->next);*/
/*}*/
