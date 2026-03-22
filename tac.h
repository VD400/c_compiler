#ifndef TAC_H
#define TAC_H

#include <stdio.h>
#include "ast.h"

typedef enum {
    TAC_ASSIGN,
    TAC_BIN_OP,
    TAC_LABEL,
    TAC_GOTO,
    TAC_IF_FALSE,
    TAC_PRINT,
    TAC_SCAN,
    TAC_RETURN,
    TAC_DECLARE,
    TAC_FUNC_BEGIN,
    TAC_FUNC_END,
    TAC_PARAM,
    TAC_CALL
} TACOpType;


typedef struct TACInstr {
    TACOpType   op;
    char*       result;
    char*       arg1;
    char*       arg2;
    char*       op_str;
    struct TACInstr* next;
} TACInstr;

typedef struct {
    TACInstr* head;
    TACInstr* tail;
} TACList;

TACList* create_tac_list(void);
char*    generate_tac(ASTNode* node, TACList* list);
void     print_tac(TACList* list);

extern FILE* tac_file;

#endif
