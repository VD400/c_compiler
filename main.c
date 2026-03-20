#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tac.h"
#include "optimizer.h"
#include "codegen.h"

FILE* token_file;
FILE* ast_file;
FILE* sym_file;
FILE* tac_file;
FILE* opt_file;
FILE* asm_file;

// parser
extern int yyparse();
extern ASTNode* root;

// symbol table
void push_scope();
void print_symbol_table();

// AST printer
void print_ast(ASTNode* node, int level);

// semantic analyzer 
extern char* analyze_semantics(ASTNode* node);
extern void  prescan_functions(ASTNode* node);

int main()
{
    token_file = fopen("tokens.txt",       "w");
    ast_file   = fopen("ast.txt",          "w");
    sym_file   = fopen("symbol_table.txt", "w");
    tac_file   = fopen("tac.txt",          "w");
    opt_file   = fopen("optimized.txt",    "w");
    asm_file   = fopen("output.asm",       "w");

    if (!token_file || !ast_file || !sym_file ||
        !tac_file   || !opt_file || !asm_file) {
        printf("Error: Could not open output files!\n");
        return 1;
    }

    printf("Starting Parsing...\n");

    // initialise global scope
    push_scope();

    if (yyparse() != 0) {
        printf("Parsing failed.\n");
        return 1;
    }

    printf("\nParsing successful!\n");

    // Stage 1: Print AST
    printf("\n--- ABSTRACT SYNTAX TREE ---\n");
    fprintf(ast_file, "\n--- ABSTRACT SYNTAX TREE ---\n");
    print_ast(root, 0);

    // Stage 2: Semantic Analysis 
    printf("\n--- SEMANTIC ANALYSIS ---\n");
    prescan_functions(root); // To register all functions first   
    analyze_semantics(root);
    print_symbol_table();

    // Stage 3: TAC Generation
    printf("\n--- INTERMEDIATE CODE GENERATION ---\n");
    TACList* tac = create_tac_list();
    generate_tac(root, tac);
    print_tac(tac);

    // Stage 4: Optimization
    TACList* optimized = optimize_tac(tac);

    // Stage 5: x86 Code Generation
    printf("\n--- CODE GENERATION ---\n");
    generate_code(optimized);

    // Close all files
    fclose(token_file);
    fclose(ast_file);
    fclose(sym_file);
    fclose(tac_file);
    fclose(opt_file);
    fclose(asm_file);

    printf("\nDone! Output files:\n");
    printf("  tokens.txt       - token stream\n");
    printf("  ast.txt          - abstract syntax tree\n");
    printf("  symbol_table.txt - symbol table\n");
    printf("  tac.txt          - three address code\n");
    printf("  optimized.txt    - optimized TAC\n");
    printf("  output.asm       - x86 assembly\n");

    return 0;
}
