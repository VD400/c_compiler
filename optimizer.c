#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "optimizer.h"
#include "tac.h"

/* -----------------------------------------------------------------------
 * Dual-output macro (same pattern as the rest of the project)
 * ----------------------------------------------------------------------- */
#define PRINT_OPT(...) do { \
    fprintf(stdout,   __VA_ARGS__); \
    fprintf(opt_file, __VA_ARGS__); \
} while(0)

/* -----------------------------------------------------------------------
 * Helpers
 * ----------------------------------------------------------------------- */

/* Returns 1 if the string is a numeric literal (integer or float) */
static int is_number(const char* s) {
    if (!s || *s == '\0') return 0;
    int i = 0;
    int has_dot = 0;
    if (s[i] == '-') i++;           /* allow leading minus */
    for (; s[i] != '\0'; i++) {
        if (s[i] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        } else if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
    }
    return i > 0;
}

/* Returns 1 if the name looks like a compiler temporary (t0, t1, t23…) */
static int is_temp(const char* s) {
    if (!s || s[0] != 't') return 0;
    for (int i = 1; s[i] != '\0'; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return s[1] != '\0';
}

/* Deep-copy a single TACInstr (next pointer set to NULL) */
static TACInstr* clone_instr(const TACInstr* src) {
    TACInstr* n = (TACInstr*)malloc(sizeof(TACInstr));
    n->op     = src->op;
    n->result = src->result ? strdup(src->result) : NULL;
    n->arg1   = src->arg1   ? strdup(src->arg1)   : NULL;
    n->arg2   = src->arg2   ? strdup(src->arg2)   : NULL;
    n->op_str = src->op_str ? strdup(src->op_str) : NULL;
    n->next   = NULL;
    return n;
}

/* Append a cloned instruction to a TACList */
static void append(TACList* list, TACInstr* instr) {
    if (!list->tail) { list->head = list->tail = instr; }
    else             { list->tail->next = instr; list->tail = instr; }
}

/* Allocate an empty TACList */
static TACList* new_list(void) {
    TACList* l = (TACList*)malloc(sizeof(TACList));
    l->head = l->tail = NULL;
    return l;
}

/* -----------------------------------------------------------------------
 * PASS 1 — Constant Folding
 *
 * For every BIN_OP instruction where both operands are numeric literals,
 * compute the result at compile time and replace the instruction with
 * a simple ASSIGN of that constant value.
 *
 * Handles: +  -  *  /  <  >  <=  >=  ==  !=
 * ----------------------------------------------------------------------- */
static TACList* pass_constant_folding(TACList* input) {
    TACList*  out   = new_list();
    int       count = 0;

    for (TACInstr* cur = input->head; cur; cur = cur->next) {

        if (cur->op == TAC_BIN_OP
            && is_number(cur->arg1)
            && is_number(cur->arg2)
            && cur->op_str) {

            double a = atof(cur->arg1);
            double b = atof(cur->arg2);
            double r = 0;
            int    is_rel = 0;   /* relational → result is 0 or 1 */
            int    rel_r  = 0;
            const char* op = cur->op_str;

            if      (strcmp(op, "+")  == 0) { r = a + b; }
            else if (strcmp(op, "-")  == 0) { r = a - b; }
            else if (strcmp(op, "*")  == 0) { r = a * b; }
            else if (strcmp(op, "/")  == 0) { r = (b != 0) ? a / b : 0; }
            else if (strcmp(op, "<")  == 0) { is_rel = 1; rel_r = (a <  b); }
            else if (strcmp(op, ">")  == 0) { is_rel = 1; rel_r = (a >  b); }
            else if (strcmp(op, "<=") == 0) { is_rel = 1; rel_r = (a <= b); }
            else if (strcmp(op, ">=") == 0) { is_rel = 1; rel_r = (a >= b); }
            else if (strcmp(op, "==") == 0) { is_rel = 1; rel_r = (a == b); }
            else if (strcmp(op, "!=") == 0) { is_rel = 1; rel_r = (a != b); }
            else {
                /* Unknown operator — keep as-is */
                append(out, clone_instr(cur));
                continue;
            }

            /* Build the folded constant string */
            char buf[64];
            if (is_rel) {
                snprintf(buf, sizeof(buf), "%d", rel_r);
            } else {
                /* Use integer representation if the result is whole */
                if (r == (long long)r) snprintf(buf, sizeof(buf), "%lld", (long long)r);
                else                   snprintf(buf, sizeof(buf), "%g",   r);
            }

            /* Replace BIN_OP with ASSIGN of the folded constant */
            TACInstr* folded = (TACInstr*)malloc(sizeof(TACInstr));
            folded->op     = TAC_ASSIGN;
            folded->result = strdup(cur->result);
            folded->arg1   = strdup(buf);
            folded->arg2   = NULL;
            folded->op_str = NULL;
            folded->next   = NULL;
            append(out, folded);
            count++;

        } else {
            append(out, clone_instr(cur));
        }
    }

    PRINT_OPT("  [Constant Folding]    %d instruction(s) folded\n", count);
    return out;
}

/* -----------------------------------------------------------------------
 * PASS 2 — Copy Propagation
 *
 * When we see:   dest = src   (TAC_ASSIGN, src is a variable or temp)
 * We scan every subsequent instruction and replace all occurrences of
 * dest with src, then remove the original copy instruction.
 *
 * We only propagate copies where the SOURCE is not itself reassigned
 * before the use — a simple conservative check: if src is reassigned
 * between the copy and a use, we stop propagating at that point.
 * ----------------------------------------------------------------------- */
static TACList* pass_copy_propagation(TACList* input) {
    TACList*  out   = new_list();
    int       count = 0;

    /* Work on an array for easier random access */
    int cap = 256, sz = 0;
    TACInstr** arr = (TACInstr**)malloc(cap * sizeof(TACInstr*));

    for (TACInstr* cur = input->head; cur; cur = cur->next) {
        if (sz >= cap) {
            cap *= 2;
            arr = (TACInstr**)realloc(arr, cap * sizeof(TACInstr*));
        }
        arr[sz++] = clone_instr(cur);
    }

    /* Mark which instructions to drop */
    int* drop = (int*)calloc(sz, sizeof(int));

    for (int i = 0; i < sz; i++) {
        TACInstr* ins = arr[i];

        /* Look for a plain copy:  dest = src  where src is not a literal.
         * IMPORTANT: only eliminate the copy if dest is a temporary (tN).
         * Never eliminate a copy into a user-declared variable like x or y —
         * those assignments are observable program behaviour. */
        if (ins->op == TAC_ASSIGN
            && ins->result
            && ins->arg1
            && !is_number(ins->arg1)
            && is_temp(ins->result)) {

            const char* dest = ins->result;
            const char* src  = ins->arg1;

            /* Propagate forward until dest or src is reassigned */
            int propagated = 0;
            for (int j = i + 1; j < sz; j++) {
                TACInstr* t = arr[j];

                /* If src is redefined, stop — propagation is no longer safe */
                if (t->result && strcmp(t->result, src) == 0) break;

                /* If dest is redefined, stop */
                if (t->result && strcmp(t->result, dest) == 0) break;

                /* Replace uses of dest with src in arg1 and arg2 */
                if (t->arg1 && strcmp(t->arg1, dest) == 0) {
                    free(t->arg1);
                    t->arg1 = strdup(src);
                    propagated = 1;
                }
                if (t->arg2 && strcmp(t->arg2, dest) == 0) {
                    free(t->arg2);
                    t->arg2 = strdup(src);
                    propagated = 1;
                }
                /* Also patch the condition operand in IF_FALSE */
                if (t->op == TAC_IF_FALSE
                    && t->arg1 && strcmp(t->arg1, dest) == 0) {
                    free(t->arg1);
                    t->arg1 = strdup(src);
                    propagated = 1;
                }
            }

            if (propagated) {
                drop[i] = 1;
                count++;
            }
        }
    }

    for (int i = 0; i < sz; i++) {
        if (!drop[i]) append(out, arr[i]);
        else          free(arr[i]);
    }

    free(arr);
    free(drop);

    PRINT_OPT("  [Copy Propagation]    %d copy instruction(s) eliminated\n", count);
    return out;
}

/* -----------------------------------------------------------------------
 * PASS 3 — Dead Code Elimination
 *
 * Extended to handle both temporaries AND user variables:
 *
 * A variable or temporary is "dead" at a definition point if its value
 * is never READ before it is next overwritten or the program ends.
 *
 * Algorithm — single backward scan:
 *   - Start with an empty "live" set.
 *   - Walk instructions from bottom to top.
 *   - When we see a USE  of a name → mark it live.
 *   - When we see a DEF  of a name → if not live, the instruction is dead.
 *     If live, remove it from the live set (the definition satisfies the use).
 *
 * Safety rules — we NEVER eliminate:
 *   - DECLARE instructions (needed by code generator for .bss slots)
 *   - SCAN  instructions  (side-effect: reads from stdin)
 *   - PRINT instructions  (side-effect: writes to stdout)
 *   - LABEL / GOTO / IF_FALSE (control flow — always kept)
 *   - RETURN instructions  (program output)
 * ----------------------------------------------------------------------- */

#define LIVE_CAP 256

typedef struct { char* names[LIVE_CAP]; int sz; } LiveSet;

static void live_add(LiveSet* ls, const char* n) {
    if (!n) return;
    for (int i = 0; i < ls->sz; i++)
        if (strcmp(ls->names[i], n) == 0) return;
    if (ls->sz < LIVE_CAP) ls->names[ls->sz++] = strdup(n);
}

static int live_has(LiveSet* ls, const char* n) {
    if (!n) return 0;
    for (int i = 0; i < ls->sz; i++)
        if (strcmp(ls->names[i], n) == 0) return 1;
    return 0;
}

static void live_remove(LiveSet* ls, const char* n) {
    if (!n) return;
    for (int i = 0; i < ls->sz; i++) {
        if (strcmp(ls->names[i], n) == 0) {
            free(ls->names[i]);
            ls->names[i] = ls->names[--ls->sz];
            return;
        }
    }
}

static TACList* pass_dead_code_elimination(TACList* input) {
    int cap = 256, sz = 0;
    TACInstr** arr = (TACInstr**)malloc(cap * sizeof(TACInstr*));

    for (TACInstr* cur = input->head; cur; cur = cur->next) {
        if (sz >= cap) { cap *= 2; arr = (TACInstr**)realloc(arr, cap * sizeof(TACInstr*)); }
        arr[sz++] = clone_instr(cur);
    }

    int* dead = (int*)calloc(sz, sizeof(int));
    LiveSet live;
    live.sz = 0;
    int count = 0;

    /* Backward pass */
    for (int i = sz - 1; i >= 0; i--) {
        TACInstr* ins = arr[i];

        switch (ins->op) {
            case TAC_BIN_OP:
                /* result = arg1 op arg2
                 * Dead if result is never read after this point */
                if (!live_has(&live, ins->result)) {
                    dead[i] = 1;
                    count++;
                    break;   /* don't mark args live — whole instr is dropped */
                }
                live_remove(&live, ins->result);
                live_add(&live, ins->arg1);
                live_add(&live, ins->arg2);
                break;

            case TAC_ASSIGN:
                /* dest = src
                 * Dead if dest is never read after this point.
                 * Exception: if src has a side-effect we can't drop it —
                 * but plain variable/literal assigns have no side-effects. */
                if (!live_has(&live, ins->result)) {
                    dead[i] = 1;
                    count++;
                    break;
                }
                live_remove(&live, ins->result);
                live_add(&live, ins->arg1);
                break;

            case TAC_IF_FALSE:
                live_add(&live, ins->arg1);
                break;

            case TAC_PRINT:
                /* PRINT is a side-effect — always keep it, but mark arg live */
                live_add(&live, ins->arg1);
                break;

            case TAC_RETURN:
                live_add(&live, ins->arg1);
                break;

            case TAC_SCAN:
                /* SCAN writes to result — keep it, mark result live */
                live_add(&live, ins->result);
                break;

            case TAC_LABEL:
            case TAC_GOTO:
            case TAC_DECLARE:
                /* Control flow and declarations are always kept */
                break;
        }
    }

    TACList* out = new_list();
    for (int i = 0; i < sz; i++) {
        if (!dead[i]) append(out, arr[i]);
        else          free(arr[i]);
    }

    free(arr);
    free(dead);

    PRINT_OPT("  [Dead Code Elim]      %d dead instruction(s) removed\n", count);
    return out;
}

/* -----------------------------------------------------------------------
 * Public entry point — run all three passes and print before/after summary
 * ----------------------------------------------------------------------- */
TACList* optimize_tac(TACList* input) {
    PRINT_OPT("\n--- OPTIMIZATION PASSES ---\n");

    TACList* after_cf  = pass_constant_folding(input);
    TACList* after_cp  = pass_copy_propagation(after_cf);
    TACList* after_dce = pass_dead_code_elimination(after_cp);

    /* Count instructions before and after */
    int before = 0, after = 0;
    for (TACInstr* i = input->head;      i; i = i->next) before++;
    for (TACInstr* i = after_dce->head;  i; i = i->next) after++;

    PRINT_OPT("  Total: %d → %d instructions  (%d removed)\n",
              before, after, before - after);
    PRINT_OPT("---------------------------\n");

    /* Print the optimized TAC */
    PRINT_OPT("\n--- OPTIMIZED THREE ADDRESS CODE ---\n");
    for (TACInstr* cur = after_dce->head; cur; cur = cur->next) {
        switch (cur->op) {
            case TAC_DECLARE:
                PRINT_OPT("    declare %s %s\n",
                          cur->arg1 ? cur->arg1 : "?",
                          cur->result ? cur->result : "?");
                break;
            case TAC_ASSIGN:
                PRINT_OPT("    %s = %s\n",
                          cur->result ? cur->result : "?",
                          cur->arg1   ? cur->arg1   : "?");
                break;
            case TAC_BIN_OP:
                PRINT_OPT("    %s = %s %s %s\n",
                          cur->result ? cur->result : "?",
                          cur->arg1   ? cur->arg1   : "?",
                          cur->op_str ? cur->op_str : "?",
                          cur->arg2   ? cur->arg2   : "?");
                break;
            case TAC_LABEL:
                PRINT_OPT("%s:\n", cur->result ? cur->result : "?");
                break;
            case TAC_GOTO:
                PRINT_OPT("    goto %s\n", cur->result ? cur->result : "?");
                break;
            case TAC_IF_FALSE:
                PRINT_OPT("    if_false %s goto %s\n",
                          cur->arg1   ? cur->arg1   : "?",
                          cur->result ? cur->result : "?");
                break;
            case TAC_PRINT:
                PRINT_OPT("    print %s\n", cur->arg1 ? cur->arg1 : "?");
                break;
            case TAC_SCAN:
                PRINT_OPT("    scan %s\n",  cur->result ? cur->result : "?");
                break;
            case TAC_RETURN:
                PRINT_OPT("    return %s\n", cur->arg1 ? cur->arg1 : "");
                break;
        }
    }
    PRINT_OPT("------------------------------------\n");

    return after_dce;
}
