%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Symbol table functions from symbol_table.c */
void push_scope();
void pop_scope();
void add_symbol(char* name, char* type);
void print_symbol_table();

/* Lexer */
extern int yylex();
void yyerror(const char *s);

ASTNode* root;
%}

%union {
    char* str;
    ASTNode* node;
}

/* Tokens with values */
%token <str> ID
%token <str> INT_LITERAL FLOAT_LITERAL

/* Keywords */
%token INT FLOAT_T CHAR_T
%token IF ELSE WHILE
%token RETURN BREAK CONTINUE
%token PRINT SCAN

/* Operators */
%token EQ NE LE GE

/* Precedence rules */
%left '+' '-'
%left '*' '/'
%left '<' '>' LE GE EQ NE
%right '='

/* Dangling else fix */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* Non-terminals */
%type <node> program stmt stmt_list expr declaration assignment while_stmt if_stmt block

%%

program
    : stmt_list
        {
            root = $1;
        }
    ;

stmt_list
    : stmt stmt_list
        {
            $1->next = $2;
            $$ = $1;
        }
    | stmt
        {
            $$ = $1;
        }
    ;

stmt
    : declaration ';'
        { $$ = $1; }

    | assignment ';'
        { $$ = $1; }

    | while_stmt
        { $$ = $1; }

    | if_stmt
        { $$ = $1; }

    | PRINT '(' expr ')' ';'
        {
            $$ = create_node(NODE_PRINT, NULL, $3, NULL);
        }

    | SCAN '(' ID ')' ';'
        {
            ASTNode* id = create_node(NODE_ID, $3, NULL, NULL);
            $$ = create_node(NODE_SCAN, NULL, id, NULL);
        }

    | RETURN expr ';'
        {
            $$ = create_node(NODE_RETURN, NULL, $2, NULL);
        }

    | BREAK ';'
        {
            $$ = create_node(NODE_BREAK, NULL, NULL, NULL);
        }

    | CONTINUE ';'
        {
            $$ = create_node(NODE_CONTINUE, NULL, NULL, NULL);
        }

    | block
        { $$ = $1; }
    ;

block
    : '{'
        {
            push_scope();
        }
      stmt_list
      '}'
        {
            pop_scope();
            $$ = $3;
        }
    ;

declaration
    : INT ID
        {
            add_symbol($2, "int");
            $$ = create_var_node("int", $2);
        }

    | FLOAT_T ID
        {
            add_symbol($2, "float");
            $$ = create_var_node("float", $2);
        }

    | CHAR_T ID
        {
            add_symbol($2, "char");
            $$ = create_var_node("char", $2);
        }
    ;

assignment
    : ID '=' expr
        {
            ASTNode* id = create_node(NODE_ID, $1, NULL, NULL);
            $$ = create_node(NODE_ASSIGN, "=", id, $3);
        }
    ;

while_stmt
    : WHILE '(' expr ')' block
        {
            $$ = create_while_node($3, $5);
        }
    ;

if_stmt
    : IF '(' expr ')' stmt %prec LOWER_THAN_ELSE
        {
            $$ = create_if_node($3, $5, NULL);
        }

    | IF '(' expr ')' stmt ELSE stmt
        {
            $$ = create_if_node($3, $5, $7);
        }
    ;

expr
    : expr '+' expr
        { $$ = create_node(NODE_BIN_OP, "+", $1, $3); }

    | expr '-' expr
        { $$ = create_node(NODE_BIN_OP, "-", $1, $3); }

    | expr '*' expr
        { $$ = create_node(NODE_BIN_OP, "*", $1, $3); }

    | expr '/' expr
        { $$ = create_node(NODE_BIN_OP, "/", $1, $3); }

    | expr '<' expr
        { $$ = create_node(NODE_BIN_OP, "<", $1, $3); }

    | expr '>' expr
        { $$ = create_node(NODE_BIN_OP, ">", $1, $3); }

    | expr LE expr
        { $$ = create_node(NODE_BIN_OP, "<=", $1, $3); }

    | expr GE expr
        { $$ = create_node(NODE_BIN_OP, ">=", $1, $3); }

    | expr EQ expr
        { $$ = create_node(NODE_BIN_OP, "==", $1, $3); }

    | expr NE expr
        { $$ = create_node(NODE_BIN_OP, "!=", $1, $3); }

    | '(' expr ')'
        { $$ = $2; }

    | ID
        {
            $$ = create_node(NODE_ID, $1, NULL, NULL);
        }

    | INT_LITERAL
        {
            $$ = create_node(NODE_INT, $1, NULL, NULL);
        }

    | FLOAT_LITERAL
        {
            $$ = create_node(NODE_FLOAT, $1, NULL, NULL);
        }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Parser error: %s\n", s);
}
