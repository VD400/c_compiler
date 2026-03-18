#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

// Fully define the struct here so semantic.c can access sym->data_type
typedef struct Symbol {
    char* name;
    char* data_type;
    struct Symbol* next;
} Symbol;

void push_scope();
void pop_scope();

void add_symbol(char* name, char* type);
Symbol* lookup_symbol(char* name);

void print_symbol_table();

#endif




/*#ifndef SYMBOL_TABLE_H*/
/*#define SYMBOL_TABLE_H*/

/*typedef struct Symbol Symbol;*/

/*void push_scope();*/
/*void pop_scope();*/

/*void add_symbol(char* name, char* type);*/
/*Symbol* lookup_symbol(char* name);*/

/*void print_symbol_table();*/

/*#endif*/
