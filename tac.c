#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "ast.h"

#define PRINT_TAC(...) do { \
    fprintf(stdout,   __VA_ARGS__); \
    fprintf(tac_file, __VA_ARGS__); \
} while(0)

static int temp_count  = 0;
static int label_count = 0;

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
 * Scope-aware variable renaming table
 *
 * Problem: if the user declares int x in an outer scope and then int x
 * again in an inner block, both would map to _var_x in the assembly,
 * causing the inner assignment to overwrite the outer variable.
 *
 * Solution: maintain a stack of (original_name -> tac_name) mappings,
 * one frame per block scope. When a variable is declared, if the name
 * already exists in an outer scope, rename it to name_N (e.g. x_1).
 * All uses of x inside the inner scope resolve to the renamed version.
 * ----------------------------------------------------------------------- */
#define RENAME_MAX  256   /* max variables per scope frame              */
#define SCOPE_MAX    64   /* max nesting depth                          */

typedef struct {
    char* original;   /* name as written in source, e.g. "x"           */
    char* tac_name;   /* name used in TAC,           e.g. "x" or "x_1" */
} RenameEntry;

typedef struct {
    RenameEntry entries[RENAME_MAX];
    int         sz;
} ScopeFrame;

static ScopeFrame scope_stack[SCOPE_MAX];
static int        scope_top = 0;   /* index of current frame (0 = global) */
static int        rename_counter = 0;

static void scope_push(void) {
    if (scope_top + 1 < SCOPE_MAX) {
        scope_top++;
        scope_stack[scope_top].sz = 0;
    }
}

static void scope_pop(void) {
    if (scope_top > 0) {
        ScopeFrame* f = &scope_stack[scope_top];
        for (int i = 0; i < f->sz; i++) {
            free(f->entries[i].original);
            free(f->entries[i].tac_name);
        }
        f->sz = 0;
        scope_top--;
    }
}

/* Register a variable declaration in the current scope.
 * If the name already exists in any outer scope, generate a unique
 * TAC name (name_N) so the two variables get separate memory slots.
 * Returns the TAC name to use for this variable. */
static const char* scope_declare(const char* name) {
    /* Check if name exists in any scope (current or outer) */
    int found_in_outer = 0;
    for (int s = 0; s < scope_top; s++) {  /* don't check current frame */
        for (int i = 0; i < scope_stack[s].sz; i++) {
            if (strcmp(scope_stack[s].entries[i].original, name) == 0) {
                found_in_outer = 1;
                break;
            }
        }
        if (found_in_outer) break;
    }

    char tac_name[128];
    if (found_in_outer) {
        /* Create a unique name so this variable gets its own memory slot */
        snprintf(tac_name, sizeof(tac_name), "%s_%d", name, rename_counter++);
    } else {
        snprintf(tac_name, sizeof(tac_name), "%s", name);
    }

    /* Register in current scope frame */
    ScopeFrame* f = &scope_stack[scope_top];
    if (f->sz < RENAME_MAX) {
        f->entries[f->sz].original = strdup(name);
        f->entries[f->sz].tac_name = strdup(tac_name);
        f->sz++;
    }

    return scope_stack[scope_top].entries[scope_stack[scope_top].sz - 1].tac_name;
}

/* Look up the TAC name for a variable use.
 * Searches from innermost scope outward (innermost wins = shadowing). */
static const char* scope_lookup(const char* name) {
    for (int s = scope_top; s >= 0; s--) {
        for (int i = 0; i < scope_stack[s].sz; i++) {
            if (strcmp(scope_stack[s].entries[i].original, name) == 0) {
                return scope_stack[s].entries[i].tac_name;
            }
        }
    }
    return name;  /* fallback: use as-is (should not happen after sem. analysis) */
}

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
    TACInstr* i = (TACInstr*)malloc(sizeof(TACInstr));
    i->op     = op;
    i->result = result ? strdup(result) : NULL;
    i->arg1   = arg1   ? strdup(arg1)   : NULL;
    i->arg2   = arg2   ? strdup(arg2)   : NULL;
    i->op_str = op_str ? strdup(op_str) : NULL;
    i->next   = NULL;
    return i;
}

static void emit(TACList* list, TACInstr* instr) {
    if (!list->tail) { list->head = list->tail = instr; }
    else             { list->tail->next = instr; list->tail = instr; }
}

static void emit_assign   (TACList* l, const char* r, const char* s)              { emit(l, make_instr(TAC_ASSIGN,     r, s, NULL, NULL)); }
static void emit_binop    (TACList* l, const char* r, const char* a, const char* op, const char* b) { emit(l, make_instr(TAC_BIN_OP, r, a, b, op)); }
static void emit_label    (TACList* l, const char* lb)                             { emit(l, make_instr(TAC_LABEL,      lb, NULL, NULL, NULL)); }
static void emit_goto     (TACList* l, const char* lb)                             { emit(l, make_instr(TAC_GOTO,       lb, NULL, NULL, NULL)); }
static void emit_if_false (TACList* l, const char* c, const char* lb)             { emit(l, make_instr(TAC_IF_FALSE,   lb, c,    NULL, NULL)); }
static void emit_print    (TACList* l, const char* v)                              { emit(l, make_instr(TAC_PRINT,      NULL, v,  NULL, NULL)); }
static void emit_scan     (TACList* l, const char* v)                              { emit(l, make_instr(TAC_SCAN,       v,    NULL, NULL, NULL)); }
static void emit_return   (TACList* l, const char* v)                              { emit(l, make_instr(TAC_RETURN,     NULL, v,   NULL, NULL)); }
static void emit_declare  (TACList* l, const char* ty, const char* nm)            { emit(l, make_instr(TAC_DECLARE,    nm,   ty,  NULL, NULL)); }
static void emit_func_begin(TACList* l, const char* nm)                            { emit(l, make_instr(TAC_FUNC_BEGIN, nm,   NULL,NULL, NULL)); }
static void emit_func_end  (TACList* l, const char* nm)                            { emit(l, make_instr(TAC_FUNC_END,   nm,   NULL,NULL, NULL)); }
static void emit_param    (TACList* l, const char* v)                              { emit(l, make_instr(TAC_PARAM,      NULL, v,   NULL, NULL)); }

/* loop label stack */
#define LOOP_MAX 64
static struct { char* start; char* end; } loop_stack[LOOP_MAX];
static int loop_depth = 0;

static void push_loop(const char* s, const char* e) {
    loop_stack[loop_depth].start = strdup(s);
    loop_stack[loop_depth].end   = strdup(e);
    loop_depth++;
}
static void pop_loop(void) {
    if (loop_depth == 0) return;
    loop_depth--;
    free(loop_stack[loop_depth].start);
    free(loop_stack[loop_depth].end);
}
static const char* cur_loop_end(void)   { return loop_stack[loop_depth-1].end; }
static const char* cur_loop_start(void) { return loop_stack[loop_depth-1].start; }

/* -----------------------------------------------------------------------
 * Main recursive generator
 * ----------------------------------------------------------------------- */
char* generate_tac(ASTNode* node, TACList* list) {
    if (!node) return NULL;

    char* result = NULL;

    switch (node->type) {

        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_STR:
            result = strdup(node->value);
            break;

        case NODE_ID:
            result = strdup(scope_lookup(node->value));
            break;

        case NODE_VAR_DECL: {
            const char* tac_name = scope_declare(node->value);
            emit_declare(list, node->data_type, tac_name);
            result = strdup(tac_name);
            break;
        }

        case NODE_BIN_OP:
	case NODE_REL_OP:
	case NODE_LOGIC_OP: {
    	    if (strcmp(node->value, "!") == 0) {
        	char* v   = generate_tac(node->left, list);
        	char* tmp = new_temp();
        	emit_binop(list, tmp, v, "==", "0");
        	free(v);
        	result = tmp;
        	break;
    	    }
    	    char* lv = generate_tac(node->left,  list);
            char* rv = generate_tac(node->right, list);
    	    char* tmp = new_temp();
            emit_binop(list, tmp, lv, node->value, rv);
            free(lv); free(rv);
            result = tmp;
            break;
}

        case NODE_ASSIGN: {
            char* rhs = generate_tac(node->right, list);
            const char* lhs = scope_lookup(node->left->value);
            emit_assign(list, lhs, rhs);
            free(rhs);
            result = strdup(lhs);
            break;
        }

        case NODE_IF: {
            char* cond  = generate_tac(node->condition, list);
            char* l_false = new_label();
            char* l_end   = new_label();
            emit_if_false(list, cond, l_false);
            free(cond);
            generate_tac(node->left, list);
            if (node->else_body) emit_goto(list, l_end);
            emit_label(list, l_false);
            if (node->else_body) {
                generate_tac(node->else_body, list);
                emit_label(list, l_end);
            }
            free(l_false); free(l_end);
            break;
        }

        case NODE_WHILE: {
            char* l_start = new_label();
            char* l_end   = new_label();
            push_loop(l_start, l_end);
            emit_label(list, l_start);
            char* cond = generate_tac(node->condition, list);
            emit_if_false(list, cond, l_end);
            free(cond);
            generate_tac(node->left, list);
            emit_goto(list, l_start);
            emit_label(list, l_end);
            pop_loop();
            free(l_start); free(l_end);
            break;
        }

        case NODE_BLOCK:
            scope_push();
            generate_tac(node->left, list);
            scope_pop();
            break;

        case NODE_PRINT: {
            if (node->value && !node->left) {
                /* print("string literal") — value holds the string text */
                /* Use result field to carry the string, arg1 stays NULL */
                emit(list, make_instr(TAC_PRINT, node->value, NULL, NULL, NULL));
            } else {
                char* v = generate_tac(node->left, list);
                emit_print(list, v);
                free(v);
            }
            break;
        }

        case NODE_SCAN:
            emit_scan(list, scope_lookup(node->left->value));
            break;

        case NODE_RETURN: {
            char* v = generate_tac(node->left, list);
            emit_return(list, v);
            if (v) free(v);
            break;
        }

        case NODE_BREAK:
            emit_goto(list, cur_loop_end());
            break;

        case NODE_CONTINUE:
            emit_goto(list, cur_loop_start());
            break;

        case NODE_STMT_LST:
            generate_tac(node->left, list);
            break;

        /* ----------------------------------------------------------------
         * FUNCTION DEFINITION
         *
         * Emits:
         *   func_begin add
         *   declare int a      ← one per parameter
         *   declare int b
         *   <body TAC>
         *   func_end add
         * ---------------------------------------------------------------- */
        case NODE_FUNC_DEF: {
            emit_func_begin(list, node->value);
            scope_push();

            /* Declare each parameter in the function scope */
            ASTNode* p = node->params;
            while (p) {
                const char* pname = scope_declare(p->value);
                emit_declare(list, p->data_type, pname);
                p = p->next;
            }

            /* Generate the function body */
            generate_tac(node->left, list);

            scope_pop();
            emit_func_end(list, node->value);
            break;
        }

        /* ----------------------------------------------------------------
         * FUNCTION CALL
         *
         * Emits:
         *   param x       ← one per argument, left to right
         *   param y
         *   t0 = call add 2
         *
         * Returns the temp that holds the return value.
         * ---------------------------------------------------------------- */
        case NODE_FUNC_CALL: {
            /* Count and emit each argument */
            int argc = 0;
            ASTNode* arg = node->args;
            while (arg) {
                char* argv = generate_tac(arg, list);
                emit_param(list, argv);
                free(argv);
                argc++;
                arg = arg->next;
            }

            /* Emit the call — result goes in a fresh temp */
            char* ret_tmp = new_temp();
            char  argc_str[16];
            snprintf(argc_str, sizeof(argc_str), "%d", argc);
            emit(list, make_instr(TAC_CALL, ret_tmp, node->value, argc_str, NULL));
            result = ret_tmp;
            break;
        }

        default:
            generate_tac(node->left,  list);
            generate_tac(node->right, list);
            break;
    }

    if (node->next) generate_tac(node->next, list);

    return result;
}

/* -----------------------------------------------------------------------
 * Pretty printer
 * ----------------------------------------------------------------------- */
void print_tac(TACList* list) {
    PRINT_TAC("\n--- THREE ADDRESS CODE ---\n");
    for (TACInstr* i = list->head; i; i = i->next) {
        switch (i->op) {
            case TAC_DECLARE:
                PRINT_TAC("    declare %s %s\n",
                          i->arg1   ? i->arg1   : "?",
                          i->result ? i->result : "?");
                break;
            case TAC_ASSIGN:
                PRINT_TAC("    %s = %s\n",
                          i->result ? i->result : "?",
                          i->arg1   ? i->arg1   : "?");
                break;
            case TAC_BIN_OP:
                PRINT_TAC("    %s = %s %s %s\n",
                          i->result ? i->result : "?",
                          i->arg1   ? i->arg1   : "?",
                          i->op_str ? i->op_str : "?",
                          i->arg2   ? i->arg2   : "?");
                break;
            case TAC_LABEL:
                PRINT_TAC("%s:\n", i->result ? i->result : "?");
                break;
            case TAC_GOTO:
                PRINT_TAC("    goto %s\n", i->result ? i->result : "?");
                break;
            case TAC_IF_FALSE:
                PRINT_TAC("    if_false %s goto %s\n",
                          i->arg1   ? i->arg1   : "?",
                          i->result ? i->result : "?");
                break;
            case TAC_PRINT:
                if (i->result && !i->arg1)
                    PRINT_TAC("    print \"%s\"\n", i->result);
                else
                    PRINT_TAC("    print %s\n", i->arg1 ? i->arg1 : "?");
                break;
            case TAC_SCAN:
                PRINT_TAC("    scan %s\n",  i->result ? i->result : "?");
                break;
            case TAC_RETURN:
                PRINT_TAC("    return %s\n", i->arg1 ? i->arg1 : "");
                break;
            case TAC_FUNC_BEGIN:
                PRINT_TAC("\nfunc_begin %s\n", i->result ? i->result : "?");
                break;
            case TAC_FUNC_END:
                PRINT_TAC("func_end %s\n\n", i->result ? i->result : "?");
                break;
            case TAC_PARAM:
                PRINT_TAC("    param %s\n", i->arg1 ? i->arg1 : "?");
                break;
            case TAC_CALL:
                PRINT_TAC("    %s = call %s %s\n",
                          i->result ? i->result : "?",
                          i->arg1   ? i->arg1   : "?",
                          i->arg2   ? i->arg2   : "0");
                break;
        }
    }
    PRINT_TAC("--------------------------\n");
}
