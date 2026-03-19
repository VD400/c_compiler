#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen.h"
#include "tac.h"

/* -----------------------------------------------------------------------
 * Dual-output macro
 * ----------------------------------------------------------------------- */
#define EMIT(...) do { \
    fprintf(stdout,   __VA_ARGS__); \
    fprintf(asm_file, __VA_ARGS__); \
} while(0)

/* -----------------------------------------------------------------------
 * Symbol map — tracks every variable/temporary that needs a memory slot
 *
 * We use a simple flat array.  For a college-project compiler this is
 * more than sufficient — real compilers use hash tables here.
 * ----------------------------------------------------------------------- */
#define MAX_SYMS 512

typedef struct {
    char* name;       /* variable or temporary name, e.g. "x" or "t0" */
    char* asm_label;  /* the label used in the .bss section            */
    int   is_float;   /* 1 if declared as float, 0 otherwise           */
} SymEntry;

static SymEntry sym_table[MAX_SYMS];
static int      sym_count = 0;

/* Look up a name; return its asm_label, or NULL if not found */
static const char* sym_lookup(const char* name) {
    for (int i = 0; i < sym_count; i++)
        if (strcmp(sym_table[i].name, name) == 0)
            return sym_table[i].asm_label;
    return NULL;
}

/* Register a new name and assign it a unique .bss label */
static const char* sym_register(const char* name, int is_float) {
    if (sym_lookup(name)) return sym_lookup(name);
    if (sym_count >= MAX_SYMS) {
        fprintf(stderr, "Codegen Error: symbol table full\n");
        exit(1);
    }
    char label[64];
    /* Use the variable name directly for declared variables;
       use _tN for compiler temporaries so they don't clash */
    if (name[0] == 't' && isdigit((unsigned char)name[1]))
        snprintf(label, sizeof(label), "_tmp_%s", name);
    else
        snprintf(label, sizeof(label), "_var_%s", name);

    sym_table[sym_count].name      = strdup(name);
    sym_table[sym_count].asm_label = strdup(label);
    sym_table[sym_count].is_float  = is_float;
    sym_count++;
    return sym_table[sym_count - 1].asm_label;
}

/* -----------------------------------------------------------------------
 * Helper: is this string a numeric literal?
 * ----------------------------------------------------------------------- */
static int is_number(const char* s) {
    if (!s || *s == '\0') return 0;
    int i = 0, dots = 0;
    if (s[i] == '-') i++;
    for (; s[i]; i++) {
        if (s[i] == '.') { if (dots++) return 0; }
        else if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

static int is_float_literal(const char* s) {
    if (!is_number(s)) return 0;
    return strchr(s, '.') != NULL;
}

/* Returns 1 if the string looks like a generated label (L0, L1, L23…) */
static int is_label_name(const char* s) {
    if (!s || s[0] != 'L') return 0;
    for (int i = 1; s[i] != '\0'; i++)
        if (!isdigit((unsigned char)s[i])) return 0;
    return s[1] != '\0';
}

/* -----------------------------------------------------------------------
 * Pre-scan pass — collect all variables and temporaries that appear in
 * the TAC so we can emit .bss reservations before the code section.
 * Also pre-scan DECLARE instructions to know which names are floats.
 * ----------------------------------------------------------------------- */
static void prescan(TACList* list) {
    /* First pass: handle declarations to get type info */
    for (TACInstr* cur = list->head; cur; cur = cur->next) {
        if (cur->op == TAC_DECLARE && cur->result && cur->arg1) {
            int is_float = (strcmp(cur->arg1, "float") == 0);
            sym_register(cur->result, is_float);
        }
    }

    /* Second pass: register anything else that's not a literal or label.
     * Skip:  TAC_LABEL  (definition of a label)
     *        TAC_GOTO   (target is a label name, not a variable)
     *        TAC_DECLARE (already handled above)
     *        IF_FALSE result field (that's the jump target label, not a var) */
    for (TACInstr* cur = list->head; cur; cur = cur->next) {
        /* result field — skip labels, gotos, declares, and label-name strings */
        if (cur->result
            && cur->op != TAC_LABEL
            && cur->op != TAC_GOTO
            && cur->op != TAC_DECLARE
            && cur->op != TAC_IF_FALSE) {   /* IF_FALSE result = jump target */
            if (!is_number(cur->result)
                && !is_label_name(cur->result)
                && !sym_lookup(cur->result))
                sym_register(cur->result, 0);
        }
        /* arg1 field */
        if (cur->arg1
            && cur->op != TAC_DECLARE) {
            if (!is_number(cur->arg1)
                && !is_label_name(cur->arg1)
                && !sym_lookup(cur->arg1))
                sym_register(cur->arg1, 0);
        }
        /* arg2 field */
        if (cur->arg2) {
            if (!is_number(cur->arg2)
                && !is_label_name(cur->arg2)
                && !sym_lookup(cur->arg2))
                sym_register(cur->arg2, 0);
        }
    }
}

/* -----------------------------------------------------------------------
 * Emit a value into eax
 *
 * If the operand is a numeric literal  →  movl $value, %eax
 * If it is a known variable/temp        →  movl label, %eax
 * ----------------------------------------------------------------------- */
static void load_to_eax(const char* operand) {
    if (!operand) return;
    if (is_number(operand)) {
        /* Convert float literals to integer representation for integer regs */
        if (is_float_literal(operand)) {
            long long iv = (long long)atof(operand);
            EMIT("    movl    $%lld, %%eax\n", iv);
        } else {
            EMIT("    movl    $%s, %%eax\n", operand);
        }
    } else {
        const char* lbl = sym_lookup(operand);
        if (lbl) EMIT("    movl    %s, %%eax\n", lbl);
        else     EMIT("    movl    $0, %%eax          # unknown: %s\n", operand);
    }
}

/* Emit a value into ebx */
static void load_to_ebx(const char* operand) {
    if (!operand) return;
    if (is_number(operand)) {
        if (is_float_literal(operand)) {
            long long iv = (long long)atof(operand);
            EMIT("    movl    $%lld, %%ebx\n", iv);
        } else {
            EMIT("    movl    $%s, %%ebx\n", operand);
        }
    } else {
        const char* lbl = sym_lookup(operand);
        if (lbl) EMIT("    movl    %s, %%ebx\n", lbl);
        else     EMIT("    movl    $0, %%ebx          # unknown: %s\n", operand);
    }
}

/* Store eax into the memory slot for a name */
static void store_from_eax(const char* name) {
    if (!name) return;
    const char* lbl = sym_lookup(name);
    if (!lbl) {
        sym_register(name, 0);
        lbl = sym_lookup(name);
    }
    EMIT("    movl    %%eax, %s\n", lbl);
}

/* -----------------------------------------------------------------------
 * Emit the binary operation currently in eax (left) and ebx (right)
 * Result lands in eax.
 *
 * Arithmetic:   ADD, SUB, IMUL, IDIV
 * Relational:   CMP + SETcc → result is 0 or 1 in eax
 * ----------------------------------------------------------------------- */
static void emit_binop(const char* op) {
    if (!op) return;

    if (strcmp(op, "+") == 0) {
        EMIT("    addl    %%ebx, %%eax\n");

    } else if (strcmp(op, "-") == 0) {
        EMIT("    subl    %%ebx, %%eax\n");

    } else if (strcmp(op, "*") == 0) {
        EMIT("    imull   %%ebx, %%eax\n");

    } else if (strcmp(op, "/") == 0) {
        /* idivl divides edx:eax by the operand; quotient → eax */
        EMIT("    movl    %%ebx, %%ecx\n");   /* save divisor */
        EMIT("    cdq\n");                     /* sign-extend eax into edx */
        EMIT("    idivl   %%ecx\n");           /* eax = eax / ecx */

    } else if (strcmp(op, "<") == 0) {
        EMIT("    cmpl    %%ebx, %%eax\n");
        EMIT("    setl    %%al\n");
        EMIT("    movzbl  %%al, %%eax\n");

    } else if (strcmp(op, ">") == 0) {
        EMIT("    cmpl    %%ebx, %%eax\n");
        EMIT("    setg    %%al\n");
        EMIT("    movzbl  %%al, %%eax\n");

    } else if (strcmp(op, "<=") == 0) {
        EMIT("    cmpl    %%ebx, %%eax\n");
        EMIT("    setle   %%al\n");
        EMIT("    movzbl  %%al, %%eax\n");

    } else if (strcmp(op, ">=") == 0) {
        EMIT("    cmpl    %%ebx, %%eax\n");
        EMIT("    setge   %%al\n");
        EMIT("    movzbl  %%al, %%eax\n");

    } else if (strcmp(op, "==") == 0) {
        EMIT("    cmpl    %%ebx, %%eax\n");
        EMIT("    sete    %%al\n");
        EMIT("    movzbl  %%al, %%eax\n");

    } else if (strcmp(op, "!=") == 0) {
        EMIT("    cmpl    %%ebx, %%eax\n");
        EMIT("    setne   %%al\n");
        EMIT("    movzbl  %%al, %%eax\n");

    } else {
        EMIT("    # Unknown operator: %s\n", op);
    }
}

/* -----------------------------------------------------------------------
 * Main code generator
 * ----------------------------------------------------------------------- */
void generate_code(TACList* list) {

    /* ---- Pre-scan to collect all symbols ---- */
    prescan(list);

    EMIT("\n--- X86 ASSEMBLY OUTPUT ---\n\n");

    /* ================================================================
     * .data section — format strings for print / scan
     * ================================================================ */
    EMIT(".section .data\n");
    EMIT("    fmt_int:   .string \"%%d\\n\"\n");
    EMIT("    fmt_float: .string \"%%f\\n\"\n");
    EMIT("    fmt_scan:  .string \"%%d\"\n");
    EMIT("\n");

    /* ================================================================
     * .bss section — one DWORD slot per variable / temporary
     * ================================================================ */
    EMIT(".section .bss\n");
    for (int i = 0; i < sym_count; i++) {
        EMIT("    %s: .space 4\n", sym_table[i].asm_label);
    }
    EMIT("\n");

    /* ================================================================
     * .text section — the actual instructions
     * ================================================================ */
    EMIT(".section .text\n");
    EMIT(".globl main\n");
    EMIT("main:\n");
    EMIT("    pushl   %%ebp\n");
    EMIT("    movl    %%esp, %%ebp\n");
    EMIT("\n");

    /* ================================================================
     * Translate each TAC instruction
     * ================================================================ */
    for (TACInstr* cur = list->head; cur; cur = cur->next) {

        switch (cur->op) {

            /* ---- DECLARE: nothing to emit — already in .bss ---- */
            case TAC_DECLARE:
                EMIT("    # declare %s %s\n",
                     cur->arg1 ? cur->arg1 : "",
                     cur->result ? cur->result : "");
                break;

            /* ---- ASSIGN: result = arg1 ---- */
            /*   load arg1 → eax, store eax → result                */
            case TAC_ASSIGN:
                EMIT("    # %s = %s\n",
                     cur->result ? cur->result : "",
                     cur->arg1   ? cur->arg1   : "");
                load_to_eax(cur->arg1);
                store_from_eax(cur->result);
                break;

            /* ---- BIN_OP: result = arg1 op arg2 ---- */
            /*   load arg1 → eax, arg2 → ebx, apply op, store eax  */
            case TAC_BIN_OP:
                EMIT("    # %s = %s %s %s\n",
                     cur->result ? cur->result : "",
                     cur->arg1   ? cur->arg1   : "",
                     cur->op_str ? cur->op_str : "",
                     cur->arg2   ? cur->arg2   : "");
                load_to_eax(cur->arg1);
                load_to_ebx(cur->arg2);
                emit_binop(cur->op_str);
                store_from_eax(cur->result);
                break;

            /* ---- LABEL: emit a jump target ---- */
            case TAC_LABEL:
                EMIT("%s:\n", cur->result ? cur->result : "L_unknown");
                break;

            /* ---- GOTO: unconditional jump ---- */
            case TAC_GOTO:
                EMIT("    jmp     %s\n", cur->result ? cur->result : "");
                break;

            /* ---- IF_FALSE: conditional jump ---- */
            /*   if_false cond goto label                            */
            /*   load cond → eax, test eax, jump if zero            */
            case TAC_IF_FALSE:
                EMIT("    # if_false %s goto %s\n",
                     cur->arg1   ? cur->arg1   : "",
                     cur->result ? cur->result : "");
                load_to_eax(cur->arg1);
                EMIT("    testl   %%eax, %%eax\n");
                EMIT("    je      %s\n", cur->result ? cur->result : "");
                break;

            /* ---- PRINT: call printf with the value ---- */
            /*   We push the value and the format string, call printf */
            case TAC_PRINT:
                EMIT("    # print %s\n", cur->arg1 ? cur->arg1 : "");
                load_to_eax(cur->arg1);
                EMIT("    pushl   %%eax\n");
                EMIT("    pushl   $fmt_int\n");
                EMIT("    call    printf\n");
                EMIT("    addl    $8, %%esp\n");   /* clean up 2 args × 4 bytes */
                break;

            /* ---- SCAN: call scanf to read into a variable ---- */
            case TAC_SCAN: {
                EMIT("    # scan %s\n", cur->result ? cur->result : "");
                const char* lbl = sym_lookup(cur->result);
                if (!lbl) { sym_register(cur->result, 0); lbl = sym_lookup(cur->result); }
                EMIT("    pushl   $%s\n", lbl);    /* address of the variable */
                EMIT("    pushl   $fmt_scan\n");
                EMIT("    call    scanf\n");
                EMIT("    addl    $8, %%esp\n");
                break;
            }

            /* ---- RETURN: load value into eax and return ---- */
            case TAC_RETURN:
                EMIT("    # return %s\n", cur->arg1 ? cur->arg1 : "");
                if (cur->arg1) load_to_eax(cur->arg1);
                else           EMIT("    movl    $0, %%eax\n");
                EMIT("    movl    %%ebp, %%esp\n");
                EMIT("    popl    %%ebp\n");
                EMIT("    ret\n");
                break;
        }

        EMIT("\n");
    }

    /* ================================================================
     * Function epilogue — in case execution falls off the end
     * ================================================================ */
    EMIT("    movl    $0, %%eax\n");
    EMIT("    movl    %%ebp, %%esp\n");
    EMIT("    popl    %%ebp\n");
    EMIT("    ret\n");
    EMIT("\n---------------------------\n");
}
