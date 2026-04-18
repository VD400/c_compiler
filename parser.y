%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
void yyerror(const char *s);

ASTNode* root;
%}

%union {
    char*    str;
    ASTNode* node;
}

%token <str> ID
%token <str> INT_LITERAL FLOAT_LITERAL STR_LITERAL CHAR_LITERAL

%token INT FLOAT_T CHAR_T
%token IF ELSE WHILE
%token RETURN BREAK CONTINUE
%token PRINT SCAN TYPE

%token EQ NE LE GE AND OR NOT
 
%right '='
%left OR
%left AND
%right NOT
%left '<' '>' LE GE EQ NE
%left '+' '-'
%left '*' '/'

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <node> program stmt stmt_list expr declaration assignment
%type <node> while_stmt if_stmt block
%type <node> func_def param_list param arg_list

%%

program
    : stmt_list
        { root = $1; }
    | func_list
        { }
    | func_list stmt_list
        {
            ASTNode* cur = root;
            while (cur->next) cur = cur->next;
            cur->next = $2;
        }
    ;

func_list
    : func_def
        {
            if (root == NULL) root = $1;
            else {
                ASTNode* cur = root;
                while (cur->next) cur = cur->next;
                cur->next = $1;
            }
        }
    | func_list func_def
        {
            ASTNode* cur = root;
            while (cur->next) cur = cur->next;
            cur->next = $2;
        }
    ;

func_def
    : INT ID '(' param_list ')' block
        { $$ = create_func_def_node("int", $2, $4, $6); }
    | FLOAT_T ID '(' param_list ')' block
        { $$ = create_func_def_node("float", $2, $4, $6); }
    | CHAR_T ID '(' param_list ')' block
        { $$ = create_func_def_node("char", $2, $4, $6); }
    | INT ID '(' ')' block
        { $$ = create_func_def_node("int", $2, NULL, $5); }
    | FLOAT_T ID '(' ')' block
        { $$ = create_func_def_node("float", $2, NULL, $5); }
    ;

param_list
    : param
        { $$ = $1; }
    | param_list ',' param
        {
            ASTNode* cur = $1;
            while (cur->next) cur = cur->next;
            cur->next = $3;
            $$ = $1;
        }
    ;

param
    : INT ID
        { $$ = create_param_node("int",   $2); }
    | FLOAT_T ID
        { $$ = create_param_node("float", $2); }
    | CHAR_T ID
        { $$ = create_param_node("char",  $2); }
    ;

stmt_list
    : stmt stmt_list
        {
            ASTNode* cur = $1;
            while(cur->next) cur = cur->next;
            cur->next = $2;
            $$ = $1;
        }
    | stmt
        { $$ = $1; }
    ;

stmt
    : declaration ';'   { $$ = $1; }
    | assignment  ';'   { $$ = $1; }
    | while_stmt        { $$ = $1; }
    | if_stmt           { $$ = $1; }
    
    
    | TYPE '(' ID ')' ';'
        { 
            ASTNode* id_node = create_node(NODE_ID, $3, NULL, NULL);
            $$ = create_node(NODE_TYPE, NULL, id_node, NULL); 
        }
        
        
    | PRINT '(' expr ')' ';'
        { $$ = create_node(NODE_PRINT, NULL, $3, NULL); }

    | PRINT '(' STR_LITERAL ')' ';'
        { $$ = create_node(NODE_PRINT, $3, NULL, NULL); }

    | SCAN '(' ID ')' ';'
        {
            ASTNode* id = create_node(NODE_ID, $3, NULL, NULL);
            $$ = create_node(NODE_SCAN, NULL, id, NULL);
        }

    | RETURN expr ';'
        { $$ = create_node(NODE_RETURN, NULL, $2, NULL); }

    | BREAK ';'
        { $$ = create_node(NODE_BREAK, NULL, NULL, NULL); }

    | CONTINUE ';'
        { $$ = create_node(NODE_CONTINUE, NULL, NULL, NULL); }

    | block
        { $$ = $1; }

    | ID '(' arg_list ')' ';'
        { $$ = create_func_call_node($1, $3); }

    | ID '(' ')' ';'
        { $$ = create_func_call_node($1, NULL); }
    ;

block
    : '{' stmt_list '}'
        { $$ = create_block_node($2); }
    ;

declaration
    : INT     ID { $$ = create_var_node("int",   $2); }
    | FLOAT_T ID { $$ = create_var_node("float", $2); }
    | CHAR_T  ID { $$ = create_var_node("char",  $2); }
    | INT     ID '=' expr { 
          ASTNode* decl = create_var_node("int", $2);
          ASTNode* id = create_node(NODE_ID, $2, NULL, NULL);
          ASTNode* assign = create_node(NODE_ASSIGN, "=", id, $4);
          decl->next = assign;
          $$ = decl;
      }
    | FLOAT_T ID '=' expr { 
          ASTNode* decl = create_var_node("float", $2);
          ASTNode* id = create_node(NODE_ID, $2, NULL, NULL);
          ASTNode* assign = create_node(NODE_ASSIGN, "=", id, $4);
          decl->next = assign;
          $$ = decl;
      }
    | CHAR_T ID '=' expr { 
          ASTNode* decl = create_var_node("char", $2);
          ASTNode* id = create_node(NODE_ID, $2, NULL, NULL);
          ASTNode* assign = create_node(NODE_ASSIGN, "=", id, $4);
          decl->next = assign;
          $$ = decl;
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
        { $$ = create_while_node($3, $5); }
    ;

if_stmt
    : IF '(' expr ')' stmt %prec LOWER_THAN_ELSE
        { $$ = create_if_node($3, $5, NULL); }
    | IF '(' expr ')' stmt ELSE stmt
        { $$ = create_if_node($3, $5, $7); }
    ;

expr
    : expr '+' expr   { $$ = create_node(NODE_BIN_OP, "+",  $1, $3); }
    | expr '-' expr   { $$ = create_node(NODE_BIN_OP, "-",  $1, $3); }
    | expr '*' expr   { $$ = create_node(NODE_BIN_OP, "*",  $1, $3); }
    | expr '/' expr   { $$ = create_node(NODE_BIN_OP, "/",  $1, $3); }
    | expr '<' expr   { $$ = create_node(NODE_BIN_OP, "<",  $1, $3); }
    | expr '>' expr   { $$ = create_node(NODE_BIN_OP, ">",  $1, $3); }
    | expr LE  expr   { $$ = create_node(NODE_BIN_OP, "<=", $1, $3); }
    | expr GE  expr   { $$ = create_node(NODE_BIN_OP, ">=", $1, $3); }
    | expr EQ  expr   { $$ = create_node(NODE_BIN_OP, "==", $1, $3); }
    | expr NE  expr   { $$ = create_node(NODE_BIN_OP, "!=", $1, $3); }
    | expr AND expr   { $$ = create_node(NODE_LOGIC_OP, "&&", $1, $3); }
    | expr OR  expr   { $$ = create_node(NODE_LOGIC_OP, "||", $1, $3); }
    | NOT expr        { $$ = create_node(NODE_LOGIC_OP, "!",  $2, NULL); }
    | '(' expr ')'    { $$ = $2; }
    | ID              { $$ = create_node(NODE_ID,    $1, NULL, NULL); }
    | INT_LITERAL     { $$ = create_node(NODE_INT,   $1, NULL, NULL); }
    | FLOAT_LITERAL   { $$ = create_node(NODE_FLOAT, $1, NULL, NULL); }
    | CHAR_LITERAL    { $$ = create_node(NODE_CHAR,  $1, NULL, NULL); }

    | ID '(' arg_list ')'
        { $$ = create_func_call_node($1, $3); }
    | ID '(' ')'
        { $$ = create_func_call_node($1, NULL); }
    ;

arg_list
    : expr
        { $$ = $1; }
    | arg_list ',' expr
        {
            ASTNode* cur = $1;
            while (cur->next) cur = cur->next;
            cur->next = $3;
            $$ = $1;
        }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parser error: %s\n", s);
}
