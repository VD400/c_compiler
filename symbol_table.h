#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* ---- Variable symbol ---- */
typedef struct Symbol {
    char* name;
    char* data_type;
    struct Symbol* next;
} Symbol;

/* ---- Parameter descriptor (for function signatures) ---- */
typedef struct ParamInfo {
    char* name;
    char* data_type;
    struct ParamInfo* next;
} ParamInfo;

/* ---- Function symbol ---- */
typedef struct FuncSymbol {
    char*       name;
    char*       return_type;
    int         param_count;
    ParamInfo*  params;          /* linked list of ParamInfo */
    struct FuncSymbol* next;
} FuncSymbol;

/* Variable scope API */
void    push_scope(void);
void    pop_scope(void);
void    add_symbol(char* name, char* type);
Symbol* lookup_symbol(char* name);
void    print_symbol_table(void);

/* Function registry API */
void        add_function(char* name, char* return_type,
                         ParamInfo* params, int param_count);
FuncSymbol* lookup_function(char* name);

#endif
