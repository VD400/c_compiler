#ifndef TAC_H
#define TAC_H

#include <stdio.h>
#include "ast.h"

/* -----------------------------------------------------------------------
 * TAC Instruction Types
 *
 *  ASSIGN      :  result = arg1
 *  BIN_OP      :  result = arg1 op arg2
 *  LABEL       :  result:
 *  GOTO        :  goto result
 *  IF_FALSE    :  if_false arg1 goto result
 *  PRINT       :  print arg1
 *  SCAN        :  scan result
 *  RETURN      :  return arg1
 *  DECLARE     :  declare type result
 *
 *  NEW — function support:
 *  FUNC_BEGIN  :  func_begin name       (marks start of function body)
 *  FUNC_END    :  func_end  name        (marks end of function body)
 *  PARAM       :  param arg1            (push one argument before a call)
 *  CALL        :  result = call name n  (call function, n args, result = retval)
 * ----------------------------------------------------------------------- */
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

/* A single Three-Address instruction */
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
