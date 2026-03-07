#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

void analyze_semantics(ASTNode* node)
{
    if(node == NULL)
        return;

    switch(node->type)
    {

        case NODE_VAR_DECL:
            add_symbol(node->value, node->data_type);
            break;

        case NODE_ID:
            if(lookup_symbol(node->value) == NULL){
                fprintf(stderr,"Semantic Error: %s not declared\n",node->value);
                exit(1);
            }
            break;

        case NODE_ASSIGN:
            analyze_semantics(node->left);
            analyze_semantics(node->right);
            break;

        case NODE_BLOCK:
            push_scope();
            analyze_semantics(node->left);
            pop_scope();
            break;

        case NODE_IF:
        case NODE_WHILE:

            analyze_semantics(node->condition);
            analyze_semantics(node->left);
            analyze_semantics(node->else_body);
            break;

        case NODE_STMT_LST:

            analyze_semantics(node->left);
            break;

        default:

            analyze_semantics(node->left);
            analyze_semantics(node->right);
            break;
    }

    analyze_semantics(node->next);
}