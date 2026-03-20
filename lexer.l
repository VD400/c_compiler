%{
#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>

extern FILE* token_file;

int line = 1;
int header_printed = 0;

#define printf(...) do {fprintf(stdout, __VA_ARGS__); fprintf(token_file, __VA_ARGS__); } while(0)

void print_header(){
    if(!header_printed){
        printf("\n%-15s %-15s %-5s\n","LEXEME","TOKEN","LINE");
        printf("-------------------------------------\n");
        header_printed = 1;
    }
}

%}

%option noyywrap nounput noinput

DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*
FLOAT   {DIGIT}+"."{DIGIT}+

%%

"int"        { print_header(); printf("%-15s %-15s %d\n",yytext,"INT",line); return INT; }
"float"      { print_header(); printf("%-15s %-15s %d\n",yytext,"FLOAT",line); return FLOAT_T; }
"char"       { print_header(); printf("%-15s %-15s %d\n",yytext,"CHAR_T",line); return CHAR_T; }

"if"         { print_header(); printf("%-15s %-15s %d\n",yytext,"IF",line); return IF; }
"else"       { print_header(); printf("%-15s %-15s %d\n",yytext,"ELSE",line); return ELSE; }
"while"      { print_header(); printf("%-15s %-15s %d\n",yytext,"WHILE",line); return WHILE; }

"return"     { print_header(); printf("%-15s %-15s %d\n",yytext,"RETURN",line); return RETURN; }
"break"      { print_header(); printf("%-15s %-15s %d\n",yytext,"BREAK",line); return BREAK; }
"continue"   { print_header(); printf("%-15s %-15s %d\n",yytext,"CONTINUE",line); return CONTINUE; }

"print"      { print_header(); printf("%-15s %-15s %d\n",yytext,"PRINT",line); return PRINT; }
"scan"       { print_header(); printf("%-15s %-15s %d\n",yytext,"SCAN",line); return SCAN; }

"=="         { print_header(); printf("%-15s %-15s %d\n",yytext,"EQ",line); return EQ; }
"!="         { print_header(); printf("%-15s %-15s %d\n",yytext,"NE",line); return NE; }
"<="         { print_header(); printf("%-15s %-15s %d\n",yytext,"LE",line); return LE; }
">="         { print_header(); printf("%-15s %-15s %d\n",yytext,"GE",line); return GE; }

"+"          { print_header(); printf("%-15s %-15s %d\n",yytext,"PLUS",line); return '+'; }
"-"          { print_header(); printf("%-15s %-15s %d\n",yytext,"MINUS",line); return '-'; }
"*"          { print_header(); printf("%-15s %-15s %d\n",yytext,"MULT",line); return '*'; }
"/"          { print_header(); printf("%-15s %-15s %d\n",yytext,"DIV",line); return '/'; }

"="          { print_header(); printf("%-15s %-15s %d\n",yytext,"ASSIGN",line); return '='; }
"<"          { print_header(); printf("%-15s %-15s %d\n",yytext,"LT",line); return '<'; }
">"          { print_header(); printf("%-15s %-15s %d\n",yytext,"GT",line); return '>'; }

";"          { print_header(); printf("%-15s %-15s %d\n",yytext,"SEMICOLON",line); return ';'; }
","          { print_header(); printf("%-15s %-15s %d\n",yytext,"COMMA",line); return ','; }
"("          { print_header(); printf("%-15s %-15s %d\n",yytext,"LPAREN",line); return '('; }
")"          { print_header(); printf("%-15s %-15s %d\n",yytext,"RPAREN",line); return ')'; }
"{"          { print_header(); printf("%-15s %-15s %d\n",yytext,"LBRACE",line); return '{'; }
"}"          { print_header(); printf("%-15s %-15s %d\n",yytext,"RBRACE",line); return '}'; }

{FLOAT}      { print_header(); printf("%-15s %-15s %d\n",yytext,"FLOAT_LITERAL",line); yylval.str=strdup(yytext); return FLOAT_LITERAL; }
{DIGIT}+     { print_header(); printf("%-15s %-15s %d\n",yytext,"INT_LITERAL",line); yylval.str=strdup(yytext); return INT_LITERAL; }

\"[^\"]*\"   {
                print_header();
                printf("%-15s %-15s %d\n", yytext, "STR_LITERAL", line);
                char* content = strdup(yytext + 1);
                content[strlen(content) - 1] = '\0';
                yylval.str = content;
                return STR_LITERAL;
             }
"&&"         { print_header(); printf("%-15s %-15s %d\n",yytext,"AND",line); return AND; }
"||"         { print_header(); printf("%-15s %-15s %d\n",yytext,"OR",line);  return OR;  }
"!"          { print_header(); printf("%-15s %-15s %d\n",yytext,"NOT",line); return NOT; }
{ID}         { print_header(); printf("%-15s %-15s %d\n",yytext,"ID",line); yylval.str=strdup(yytext); return ID; }

[ \t\r]+     ;

\n           { line++; }

.            { printf("Unknown token %s at line %d\n",yytext,line); }

%%
