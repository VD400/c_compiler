#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Symbol Symbol;

void push_scope();
void pop_scope();

void add_symbol(char* name, char* type);
Symbol* lookup_symbol(char* name);

void print_symbol_table();

#endif