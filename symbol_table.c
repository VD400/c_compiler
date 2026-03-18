#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "symbol_table.h"
extern FILE* sym_file;


#define PRINT_SYM(...) do { fprintf(stdout, __VA_ARGS__); fprintf(sym_file, __VA_ARGS__); } while(0)



typedef struct Scope{
    Symbol* symbols;
    struct Scope* outer;
}Scope;

Scope* current_scope = NULL;

void push_scope(){
    Scope* new_scope = (Scope*)malloc(sizeof(Scope));
    new_scope->symbols = NULL;
    new_scope->outer = current_scope;
    current_scope = new_scope;
/*    fprintf(sym_file,"\n    [Symbol Table] is entering new scope\n \n");*/
    PRINT_SYM("\n    [Symbol Table] is entering new scope\n \n");
}

void pop_scope(){
    if(current_scope == NULL) return;
    
    
    PRINT_SYM("\n[Symbol Table] State before exiting scope:\n");
    print_symbol_table();
    Scope* old_scope = current_scope;
    current_scope = current_scope->outer;
    Symbol* s = old_scope->symbols;
    while(s){
        Symbol* temp = s;
        s = s->next;
        free(temp->name);
        free(temp->data_type);
        free(temp);
    }
    free(old_scope);
/*    fprintf(sym_file,"\n    [Symbol Table] is exiting scope\n \n");*/
    PRINT_SYM("\n    [Symbol Table] is exiting scope\n \n");
}

void add_symbol(char* name, char* type){
    if(current_scope == NULL) 
        push_scope();
    Symbol* curr = current_scope->symbols;
    while(curr){
        if(strcmp(curr->name, name) == 0){
            fprintf(stderr, "Semantic Error: Variable '%s' already declared in this scope\n", name);
            exit(1);
        }
        curr = curr->next;
    }
    Symbol* s = (Symbol*)malloc(sizeof(Symbol));
    s->name = strdup(name);
    s->data_type = strdup(type);
    s->next = current_scope->symbols;
    current_scope->symbols = s;
/*    fprintf(sym_file,"\n    [Symbol Table] Declared '%s' as %s\n \n", name, type);*/
    PRINT_SYM("\n    [Symbol Table] Declared '%s' as %s\n \n", name, type);
}


Symbol* lookup_symbol(char* name){
    Scope* curr_scope = current_scope;
    while(curr_scope != NULL){
        Symbol* s = curr_scope->symbols;
        while(s){
            if(strcmp(s->name, name) == 0) return s;
            s = s->next;
        }
        curr_scope = curr_scope->outer; // If not found in inner scope, look at the parent scope
    }
    return NULL;
}

void print_symbol_table(){
/*    fprintf(sym_file, "\n--- SYMBOL TABLE ---\n");*/
    PRINT_SYM("\n--- SYMBOL TABLE ---\n");
    Scope* s = current_scope;
    int depth = 0;
    while(s){
/*        fprintf(sym_file,"Scope level %d:\n",depth++);*/
        PRINT_SYM("Scope level %d:\n",depth);
        depth++;
        Symbol* sym = s->symbols;
/*        if(!sym) fprintf(sym_file, "  (empty)\n");*/
        if(!sym) PRINT_SYM("  (empty)\n");
        while(sym){
/*            fprintf(sym_file, "  -> %-10s [%s]\n", sym->name, sym->data_type);*/
        PRINT_SYM("  -> %-10s [%s]\n", sym->name, sym->data_type);
            sym = sym->next;
        }
        s = s->outer;
    }
/*    fprintf(sym_file, "-----------------------------\n");*/
    PRINT_SYM("-----------------------------\n");
}
