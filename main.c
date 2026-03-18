#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

FILE* token_file;
FILE* ast_file;
FILE* sym_file;

/* parser */
extern int yyparse();
extern ASTNode* root;

/* symbol table */
void push_scope();
void print_symbol_table();

/* AST printer */
void print_ast(ASTNode* node, int level);

/* semantic analyzer */
extern char* analyze_semantics(ASTNode* node); // ADDED THIS

int main()
{
    // 1. Open the output files
    token_file = fopen("tokens.txt", "w");
    ast_file   = fopen("ast.txt", "w");
    sym_file   = fopen("symbol_table.txt", "w");
    
    if (!token_file || !ast_file || !sym_file) {
        printf("Error: Could not open output files!\n");
        return 1;
    }
    
    printf("Starting Parsing...\n");

    /* Initialize global scope */
    push_scope();

    if(yyparse() == 0)
    {
        printf("\nParsing successful!\n");
        
        printf("\n--- ABSTRACT SYNTAX TREE ---\n");

        fprintf(ast_file, "\n--- ABSTRACT SYNTAX TREE ---\n");
        print_ast(root,0);
        
        // ADDED THIS: Run the Semantic Analyzer!
        printf("\n--- SEMANTIC ANALYSIS ---\n");
        analyze_semantics(root);

        print_symbol_table();
    }
    else
    {
        printf("Parsing failed.\n");
    }
    
    fclose(token_file);
    fclose(ast_file);
    fclose(sym_file);

    return 0;
}
