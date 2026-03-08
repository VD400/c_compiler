#include <stdio.h>
#include "ast.h"

/* parser */
extern int yyparse();
extern ASTNode* root;

/* symbol table */
void push_scope();
void print_symbol_table();

/* AST printer */
void print_ast(ASTNode* node, int level);

int main()
{
    printf("Starting Parsing...\n");

    /* Initialize global scope */
    push_scope();

    if(yyparse() == 0)
    {
        printf("\nParsing successful!\n");

        printf("\n--- ABSTRACT SYNTAX TREE ---\n");
        print_ast(root,0);

        print_symbol_table();
    }
    else
    {
        printf("Parsing failed.\n");
    }

    return 0;
}
