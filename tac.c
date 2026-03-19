#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "ast.h"

/* -----------------------------------------------------------------------
 * Dual-output macro (mirrors the style used in ast.c / symbol_table.c)
 * ----------------------------------------------------------------------- */
#define PRINT_TAC(...) do { \
    fprintf(stdout,   __VA_ARGS__); \
    fprintf(tac_file, __VA_ARGS__); \
} while(0)

/* -----------------------------------------------------------------------
 * Counters for generating unique names
 * ----------------------------------------------------------------------- */
static int temp_count  = 0;   /* temporaries : t0, t1, t2, ...  */
static int label_count = 0;   /* labels      : L0, L1, L2, ...  */

/* -----------------------------------------------------------------------
 * Name-generation helpers
 * ----------------------------------------------------------------------- */
static char* new_temp(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "t%d", temp_count++);
    return strdup(buf);
}

static char* new_label(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "L%d", label_count++);
    return strdup(buf);
}

/* -----------------------------------------------------------------------
 * Instruction construction
 * ----------------------------------------------------------------------- */
TACList* create_tac_list(void) {
    TACList* list = (TACList*)malloc(sizeof(TACList));
    list->head = list->tail = NULL;
    return list;
}

static TACInstr* make_instr(TACOpType op,
                             const char* result,
                             const char* arg1,
                             const char* arg2,
                             const char* op_str) {
    TACInstr* instr = (TACInstr*)malloc(sizeof(TACInstr));
    instr->op     = op;
    instr->result = result ? strdup(result) : NULL;
    instr->arg1   = arg1   ? strdup(arg1)   : NULL;
    instr->arg2   = arg2   ? strdup(arg2)   : NULL;
    instr->op_str = op_str ? strdup(op_str) : NULL;
    instr->next   = NULL;
    return instr;
}

static void emit(TACList* list, TACInstr* instr) {
    if (list->tail == NULL) {
        list->head = list->tail = instr;
    } else {
        list->tail->next = instr;
        list->tail = instr;
    }
}

/* Convenience emitters */
static void emit_assign(TACList* l, const char* res, const char* src) {
    emit(l, make_instr(TAC_ASSIGN, res, src, NULL, NULL));
}

static void emit_binop(TACList* l, const char* res,
                       const char* a1, const char* op, const char* a2) {
    emit(l, make_instr(TAC_BIN_OP, res, a1, a2, op));
}

static void emit_label(TACList* l, const char* lbl) {
    emit(l, make_instr(TAC_LABEL, lbl, NULL, NULL, NULL));
}

static void emit_goto(TACList* l, const char* lbl) {
    emit(l, make_instr(TAC_GOTO, lbl, NULL, NULL, NULL));
}

static void emit_if_false(TACList* l, const char* cond, const char* lbl) {
    emit(l, make_instr(TAC_IF_FALSE, lbl, cond, NULL, NULL));
}

static void emit_print(TACList* l, const char* val) {
    emit(l, make_instr(TAC_PRINT, NULL, val, NULL, NULL));
}

static void emit_scan(TACList* l, const char* var) {
    emit(l, make_instr(TAC_SCAN, var, NULL, NULL, NULL));
}

static void emit_return(TACList* l, const char* val) {
    emit(l, make_instr(TAC_RETURN, NULL, val, NULL, NULL));
}

static void emit_declare(TACList* l, const char* type, const char* name) {
    emit(l, make_instr(TAC_DECLARE, name, type, NULL, NULL));
}

/* -----------------------------------------------------------------------
 * Stack for break/continue label resolution
 *
 * When we enter a while-loop we push the loop's start and end labels.
 * break  → goto end_label
 * continue → goto start_label
 * ----------------------------------------------------------------------- */
#define LOOP_STACK_MAX 64

typedef struct {
    char* start_label;
    char* end_label;
} LoopFrame;

static LoopFrame loop_stack[LOOP_STACK_MAX];
static int       loop_depth = 0;

static void push_loop(const char* start, const char* end) {
    if (loop_depth >= LOOP_STACK_MAX) {
        fprintf(stderr, "TAC Error: loop nesting too deep\n");
        exit(1);
    }
    loop_stack[loop_depth].start_label = strdup(start);
    loop_stack[loop_depth].end_label   = strdup(end);
    loop_depth++;
}

static void pop_loop(void) {
    if (loop_depth == 0) return;
    loop_depth--;
    free(loop_stack[loop_depth].start_label);
    free(loop_stack[loop_depth].end_label);
}

static const char* current_loop_start(void) {
    if (loop_depth == 0) {
        fprintf(stderr, "TAC Error: break/continue outside loop\n");
        exit(1);
    }
    return loop_stack[loop_depth - 1].start_label;
}

static const char* current_loop_end(void) {
    if (loop_depth == 0) {
        fprintf(stderr, "TAC Error: break/continue outside loop\n");
        exit(1);
    }
    return loop_stack[loop_depth - 1].end_label;
}

/* -----------------------------------------------------------------------
 * Main recursive TAC generator
 *
 * Returns the name that holds the VALUE of this node (temp or variable).
 * Returns NULL for statements that produce no value.
 * ----------------------------------------------------------------------- */
char* generate_tac(ASTNode* node, TACList* list) {
    if (node == NULL) return NULL;

    char* result = NULL;

    switch (node->type) {

        /* ---- Literals: just return their text as an operand ----------- */
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_STR:
            result = strdup(node->value);
            break;

        /* ---- Identifier: return its name as an operand ---------------- */
        case NODE_ID:
            result = strdup(node->value);
            break;

        /* ---- Variable declaration: emit a DECLARE instruction --------- */
        /*      e.g.  declare int x                                        */
        case NODE_VAR_DECL:
            emit_declare(list, node->data_type, node->value);
            result = strdup(node->value);
            break;

        /* ---- Binary / relational operations --------------------------- */
        /*      t0 = arg1 op arg2                                          */
        case NODE_BIN_OP:
        case NODE_REL_OP:
        case NODE_LOGIC_OP: {
            char* left_val  = generate_tac(node->left,  list);
            char* right_val = generate_tac(node->right, list);
            char* tmp       = new_temp();
            emit_binop(list, tmp, left_val, node->value, right_val);
            free(left_val);
            free(right_val);
            result = tmp;   /* caller owns this string */
            break;
        }

        /* ---- Assignment: x = expr ------------------------------------- */
        /*      Generates the RHS, then copies into LHS variable.         */
        case NODE_ASSIGN: {
            char* rhs = generate_tac(node->right, list);
            /* node->left is always NODE_ID for a simple assignment */
            const char* lhs = node->left->value;
            emit_assign(list, lhs, rhs);
            free(rhs);
            result = strdup(lhs);
            break;
        }

        /* ---- IF / IF-ELSE --------------------------------------------- */
        /*
         *   evaluate condition → t0
         *   if_false t0 goto L_false
         *   <true body>
         *   goto L_end          (only if there is an else branch)
         *   L_false:
         *   <else body>         (optional)
         *   L_end:
         */
        case NODE_IF: {
            char* cond_val = generate_tac(node->condition, list);

            char* l_false = new_label();
            char* l_end   = new_label();

            emit_if_false(list, cond_val, l_false);
            free(cond_val);

            /* True branch */
            generate_tac(node->left, list);

            if (node->else_body) {
                emit_goto(list, l_end);
            }

            emit_label(list, l_false);

            /* Else branch */
            if (node->else_body) {
                generate_tac(node->else_body, list);
                emit_label(list, l_end);
            }

            free(l_false);
            free(l_end);
            break;
        }

        /* ---- WHILE loop ---------------------------------------------- */
        /*
         *   L_start:
         *   evaluate condition → t0
         *   if_false t0 goto L_end
         *   <body>
         *   goto L_start
         *   L_end:
         */
        case NODE_WHILE: {
            char* l_start = new_label();
            char* l_end   = new_label();

            push_loop(l_start, l_end);

            emit_label(list, l_start);

            char* cond_val = generate_tac(node->condition, list);
            emit_if_false(list, cond_val, l_end);
            free(cond_val);

            /* Loop body */
            generate_tac(node->left, list);

            emit_goto(list, l_start);
            emit_label(list, l_end);

            pop_loop();
            free(l_start);
            free(l_end);
            break;
        }

        /* ---- Block: new scope (TAC itself is flat, no scope markers) -- */
        case NODE_BLOCK:
            generate_tac(node->left, list);
            break;

        /* ---- PRINT ----------------------------------------------------- */
        case NODE_PRINT: {
            char* val = generate_tac(node->left, list);
            emit_print(list, val);
            free(val);
            break;
        }

        /* ---- SCAN ------------------------------------------------------ */
        case NODE_SCAN:
            /* node->left is the NODE_ID that receives input */
            emit_scan(list, node->left->value);
            break;

        /* ---- RETURN ---------------------------------------------------- */
        case NODE_RETURN: {
            char* val = generate_tac(node->left, list);
            emit_return(list, val);
            if (val) free(val);
            break;
        }

        /* ---- BREAK ------------------------------------------------------ */
        case NODE_BREAK:
            emit_goto(list, current_loop_end());
            break;

        /* ---- CONTINUE --------------------------------------------------- */
        case NODE_CONTINUE:
            emit_goto(list, current_loop_start());
            break;

        /* ---- Statement list (NODE_STMT_LST) ----------------------------- */
        case NODE_STMT_LST:
            generate_tac(node->left, list);
            break;

        default:
            /* Fallback: recurse into children */
            generate_tac(node->left,  list);
            generate_tac(node->right, list);
            break;
    }

    /* Traverse the sibling chain (next statement in the same block) */
    if (node->next) {
        generate_tac(node->next, list);
    }

    return result;
}

/* -----------------------------------------------------------------------
 * Pretty printer
 *
 * Output format mirrors common compiler-textbook style:
 *   declare int x
 *   t0 = a + b
 *   x  = t0
 *   if_false t0 goto L1
 *   goto L0
 *   L1:
 *   print x
 *   scan  y
 *   return t2
 * ----------------------------------------------------------------------- */
void print_tac(TACList* list) {
    PRINT_TAC("\n--- THREE ADDRESS CODE ---\n");

    TACInstr* instr = list->head;
    while (instr) {
        switch (instr->op) {

            case TAC_DECLARE:
                PRINT_TAC("    declare %s %s\n",
                          instr->arg1   ? instr->arg1   : "?",
                          instr->result ? instr->result : "?");
                break;

            case TAC_ASSIGN:
                PRINT_TAC("    %s = %s\n",
                          instr->result ? instr->result : "?",
                          instr->arg1   ? instr->arg1   : "?");
                break;

            case TAC_BIN_OP:
                PRINT_TAC("    %s = %s %s %s\n",
                          instr->result ? instr->result : "?",
                          instr->arg1   ? instr->arg1   : "?",
                          instr->op_str ? instr->op_str : "?",
                          instr->arg2   ? instr->arg2   : "?");
                break;

            case TAC_LABEL:
                /* Labels are NOT indented — makes them stand out visually */
                PRINT_TAC("%s:\n", instr->result ? instr->result : "?");
                break;

            case TAC_GOTO:
                PRINT_TAC("    goto %s\n",
                          instr->result ? instr->result : "?");
                break;

            case TAC_IF_FALSE:
                PRINT_TAC("    if_false %s goto %s\n",
                          instr->arg1   ? instr->arg1   : "?",
                          instr->result ? instr->result : "?");
                break;

            case TAC_PRINT:
                PRINT_TAC("    print %s\n",
                          instr->arg1 ? instr->arg1 : "?");
                break;

            case TAC_SCAN:
                PRINT_TAC("    scan %s\n",
                          instr->result ? instr->result : "?");
                break;

            case TAC_RETURN:
                PRINT_TAC("    return %s\n",
                          instr->arg1 ? instr->arg1 : "");
                break;
        }
        instr = instr->next;
    }

    PRINT_TAC("--------------------------\n");
}
