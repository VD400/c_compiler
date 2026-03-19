#ifndef TAC_H
#define TAC_H

#include <stdio.h>
#include "ast.h"

/* -----------------------------------------------------------------------
 * TAC Instruction Types
 *
 *  ASSIGN      :  result = arg1
 *  BIN_OP      :  result = arg1 op arg2
 *  LABEL       :  result:               (jump target)
 *  GOTO        :  goto result
 *  IF_FALSE    :  if_false arg1 goto result
 *  PRINT       :  print arg1
 *  SCAN        :  scan  result
 *  RETURN      :  return arg1           (arg1 may be NULL)
 *  BREAK       :  goto <loop_end>       (filled in by loop handler)
 *  CONTINUE    :  goto <loop_start>     (filled in by loop handler)
 *  DECLARE     :  declare type result   (variable declaration)
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
    TAC_DECLARE
} TACOpType;

/* A single Three-Address instruction */
typedef struct TACInstr {
    TACOpType   op;
    char*       result;   /* LHS / label name / jump target */
    char*       arg1;     /* first  operand  (may be NULL)  */
    char*       arg2;     /* second operand  (may be NULL)  */
    char*       op_str;   /* operator string, e.g. "+" "-"  */
    struct TACInstr* next;
} TACInstr;

/* Linked list of all generated instructions */
typedef struct {
    TACInstr* head;
    TACInstr* tail;
} TACList;

/* Public API */
TACList* create_tac_list(void);

/* Generate TAC from an AST node; returns the name of the result temp/var */
char* generate_tac(ASTNode* node, TACList* list);

/* Pretty-print the TAC to stdout and to tac_file */
void print_tac(TACList* list);

/* File pointer — defined in main.c, used here for dual output */
extern FILE* tac_file;

#endif /* TAC_H */
