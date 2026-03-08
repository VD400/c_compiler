#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "symbol_table.h"

typedef struct Symbol{
    char* name;
    char* data_type;
    struct Symbol* next;
} Symbol;

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
    printf("\n    [Symbol Table] is entering new scope\n \n");
}

void pop_scope(){
    if(current_scope == NULL) return;
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
    printf("\n    [Symbol Table] is exiting scope\n \n");
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
    printf("\n    [Symbol Table] Declared '%s' as %s\n \n", name, type);
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
    printf("\n--- SYMBOL TABLE ---\n");
    Scope* s = current_scope;
    int depth = 0;
    while(s){
        printf("Scope level %d:\n",depth++);
        Symbol* sym = s->symbols;
        if(!sym) printf("  (empty)\n");
        while(sym){
            printf("  -> %-10s [%s]\n", sym->name, sym->data_type);
            sym = sym->next;
        }
        s = s->outer;
    }
    printf("-----------------------------\n");
}
