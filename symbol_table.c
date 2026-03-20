#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern FILE* sym_file;

#define PRINT_SYM(...) do { \
    fprintf(stdout,   __VA_ARGS__); \
    fprintf(sym_file, __VA_ARGS__); \
} while(0)


typedef struct Scope {
    Symbol*       symbols;
    struct Scope* outer;
} Scope;

static Scope* current_scope = NULL;

void push_scope(void) {
    Scope* s = (Scope*)malloc(sizeof(Scope));
    s->symbols = NULL;
    s->outer   = current_scope;
    current_scope = s;
    PRINT_SYM("\n    [Symbol Table] is entering new scope\n \n");
}

void pop_scope(void) {
    if (!current_scope) return;
    PRINT_SYM("\n[Symbol Table] State before exiting scope:\n");
    print_symbol_table();
    Scope* old = current_scope;
    current_scope = current_scope->outer;
    Symbol* s = old->symbols;
    while (s) {
        Symbol* t = s; s = s->next;
        free(t->name); free(t->data_type); free(t);
    }
    free(old);
    PRINT_SYM("\n    [Symbol Table] is exiting scope\n \n");
}

void add_symbol(char* name, char* type) {
    if (!current_scope) push_scope();
    Symbol* cur = current_scope->symbols;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            fprintf(stderr,
                    "Semantic Error: Variable '%s' already declared in this scope\n",
                    name);
            exit(1);
        }
        cur = cur->next;
    }
    Symbol* s    = (Symbol*)malloc(sizeof(Symbol));
    s->name      = strdup(name);
    s->data_type = strdup(type);
    s->next      = current_scope->symbols;
    current_scope->symbols = s;
    PRINT_SYM("\n    [Symbol Table] Declared '%s' as %s\n \n", name, type);
}

Symbol* lookup_symbol(char* name) {
    Scope* sc = current_scope;
    while (sc) {
        Symbol* s = sc->symbols;
        while (s) {
            if (strcmp(s->name, name) == 0) return s;
            s = s->next;
        }
        sc = sc->outer;
    }
    return NULL;
}

void print_symbol_table(void) {
    PRINT_SYM("\n--- SYMBOL TABLE ---\n");
    Scope* s = current_scope;
    int depth = 0;
    while (s) {
        PRINT_SYM("Scope level %d:\n", depth++);
        Symbol* sym = s->symbols;
        if (!sym) PRINT_SYM("  (empty)\n");
        while (sym) {
            PRINT_SYM("  -> %-10s [%s]\n", sym->name, sym->data_type);
            sym = sym->next;
        }
        s = s->outer;
    }
    PRINT_SYM("-----------------------------\n");
}

/* -----------------------------------------------------------------------
 * Function registry — a simple global linked list of FuncSymbol entries.
 * Functions are never "out of scope" so no stack is needed.
 * ----------------------------------------------------------------------- */
static FuncSymbol* func_table = NULL;

void add_function(char* name, char* return_type,
                  ParamInfo* params, int param_count) {
    /* Check for duplicate definition */
    FuncSymbol* cur = func_table;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            fprintf(stderr,
                    "Semantic Error: Function '%s' already defined\n", name);
            exit(1);
        }
        cur = cur->next;
    }
    FuncSymbol* f   = (FuncSymbol*)malloc(sizeof(FuncSymbol));
    f->name         = strdup(name);
    f->return_type  = strdup(return_type);
    f->params       = params;
    f->param_count  = param_count;
    f->next         = func_table;
    func_table      = f;
    PRINT_SYM("\n    [Function Table] Registered '%s' returning %s (%d param(s))\n \n",
              name, return_type, param_count);
}

FuncSymbol* lookup_function(char* name) {
    FuncSymbol* f = func_table;
    while (f) {
        if (strcmp(f->name, name) == 0) return f;
        f = f->next;
    }
    return NULL;
}
