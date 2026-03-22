#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


typedef struct Symbol {
    char* name;
    char* data_type;
    struct Symbol* next;
} Symbol;


typedef struct ParamInfo {
    char* name;
    char* data_type;
    struct ParamInfo* next;
} ParamInfo;


typedef struct FuncSymbol {
    char*       name;
    char*       return_type;
    int         param_count;
    ParamInfo*  params;         
    struct FuncSymbol* next;
} FuncSymbol;


void    push_scope(void);
void    pop_scope(void);
void    add_symbol(char* name, char* type);
Symbol* lookup_symbol(char* name);
void    print_symbol_table(void);


void        add_function(char* name, char* return_type,
                         ParamInfo* params, int param_count);
FuncSymbol* lookup_function(char* name);

#endif
