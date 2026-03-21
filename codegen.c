#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen.h"
#include "tac.h"

#define EMIT(...) do { \
    fprintf(stdout,   __VA_ARGS__); \
    fprintf(asm_file, __VA_ARGS__); \
} while(0)

/* -----------------------------------------------------------------------
 * Symbol map — variables and temporaries → .bss labels
 * ----------------------------------------------------------------------- */
#define MAX_SYMS 512

typedef struct {
    char* name;
    char* asm_label;
    int   is_float;
    int   is_char;
} SymEntry;

static SymEntry sym_table[MAX_SYMS];
static int      sym_count = 0;

/* -----------------------------------------------------------------------
 * String constant table — for print("hello") style statements
 * ----------------------------------------------------------------------- */
#define MAX_STRS 64
typedef struct { char* text; char* label; } StrEntry;
static StrEntry str_table[MAX_STRS];
static int      str_count = 0;

static const char* str_register(const char* text) {
    for (int i = 0; i < str_count; i++)
        if (strcmp(str_table[i].text, text) == 0)
            return str_table[i].label;
    if (str_count >= MAX_STRS) return "_str_overflow";
    char label[32];
    snprintf(label, sizeof(label), "_str%d", str_count);
    str_table[str_count].text  = strdup(text);
    str_table[str_count].label = strdup(label);
    str_count++;
    return str_table[str_count-1].label;
}

static const char* sym_lookup(const char* name) {
    for (int i = 0; i < sym_count; i++)
        if (strcmp(sym_table[i].name, name) == 0)
            return sym_table[i].asm_label;
    return NULL;
}

static int sym_is_char(const char* name) {
    for (int i = 0; i < sym_count; i++)
        if (strcmp(sym_table[i].name, name) == 0)
            return sym_table[i].is_char;
    return 0;
}

static const char* sym_register(const char* name, int is_float) {
    if (sym_lookup(name)) return sym_lookup(name);
    if (sym_count >= MAX_SYMS) {
        fprintf(stderr, "Codegen Error: symbol table full\n"); exit(1);
    }
    char label[64];
    if (name[0] == 't' && isdigit((unsigned char)name[1]))
        snprintf(label, sizeof(label), "_tmp_%s", name);
    else
        snprintf(label, sizeof(label), "_var_%s", name);

    sym_table[sym_count].name      = strdup(name);
    sym_table[sym_count].asm_label = strdup(label);
    sym_table[sym_count].is_float  = is_float;
    sym_table[sym_count].is_char   = 0;
    sym_count++;
    return sym_table[sym_count - 1].asm_label;
}

static const char* sym_register_typed(const char* name, int is_float, int is_char) {
    if (sym_lookup(name)) return sym_lookup(name);
    if (sym_count >= MAX_SYMS) {
        fprintf(stderr, "Codegen Error: symbol table full\n"); exit(1);
    }
    char label[64];
    if (name[0] == 't' && isdigit((unsigned char)name[1]))
        snprintf(label, sizeof(label), "_tmp_%s", name);
    else
        snprintf(label, sizeof(label), "_var_%s", name);

    sym_table[sym_count].name      = strdup(name);
    sym_table[sym_count].asm_label = strdup(label);
    sym_table[sym_count].is_float  = is_float;
    sym_table[sym_count].is_char   = is_char;
    sym_count++;
    return sym_table[sym_count - 1].asm_label;
}

/* -----------------------------------------------------------------------
 * Helpers
 * ----------------------------------------------------------------------- */
static int is_number(const char* s) {
    if (!s || !*s) return 0;
    int i = 0, dots = 0;
    if (s[i] == '-') i++;
    for (; s[i]; i++) {
        if (s[i] == '.') { if (dots++) return 0; }
        else if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

static int is_float_literal(const char* s) {
    return is_number(s) && strchr(s, '.') != NULL;
}

static int is_label_name(const char* s) {
    if (!s || s[0] != 'L') return 0;
    for (int i = 1; s[i]; i++)
        if (!isdigit((unsigned char)s[i])) return 0;
    return s[1] != '\0';
}

/* -----------------------------------------------------------------------
 * Pre-scan — register every variable/temporary in .bss
 * ----------------------------------------------------------------------- */
static void prescan(TACList* list) {
    /* First pass: declared variables — track type */
    for (TACInstr* c = list->head; c; c = c->next)
        if (c->op == TAC_DECLARE && c->result && c->arg1) {
            int is_float = strcmp(c->arg1, "float") == 0;
            int is_char  = strcmp(c->arg1, "char")  == 0;
            sym_register_typed(c->result, is_float, is_char);
        }

    /* Second pass: register string literals from print("...") statements */
    for (TACInstr* c = list->head; c; c = c->next)
        if (c->op == TAC_PRINT && c->result && !c->arg1)
            str_register(c->result);

    /* Third pass: everything else (temps, etc.) */
    for (TACInstr* c = list->head; c; c = c->next) {
        if (c->result
            && c->op != TAC_LABEL && c->op != TAC_GOTO
            && c->op != TAC_DECLARE && c->op != TAC_IF_FALSE
            && c->op != TAC_FUNC_BEGIN && c->op != TAC_FUNC_END
            && !(c->op == TAC_PRINT && !c->arg1)) {
            if (!is_number(c->result) && !is_label_name(c->result)
                && !sym_lookup(c->result))
                sym_register(c->result, 0);
        }
        if (c->arg1 && c->op != TAC_DECLARE
            && c->op != TAC_FUNC_BEGIN && c->op != TAC_FUNC_END
            && c->op != TAC_PARAM && c->op != TAC_CALL) {
            if (!is_number(c->arg1) && !is_label_name(c->arg1)
                && !sym_lookup(c->arg1))
                sym_register(c->arg1, 0);
        }
        if (c->arg2 && c->op != TAC_CALL) {
            if (!is_number(c->arg2) && !is_label_name(c->arg2)
                && !sym_lookup(c->arg2))
                sym_register(c->arg2, 0);
        }
    }
}

/* -----------------------------------------------------------------------
 * Load / store helpers
 * ----------------------------------------------------------------------- */
static void load_to_eax(const char* op) {
    if (!op) return;
    if (is_number(op)) {
        long long v = is_float_literal(op) ? (long long)atof(op) : atoll(op);
        EMIT("    movl    $%lld, %%eax\n", v);
    } else {
        const char* lbl = sym_lookup(op);
        if (lbl) EMIT("    movl    %s, %%eax\n", lbl);
        else     EMIT("    movl    $0, %%eax          # unknown: %s\n", op);
    }
}

static void load_to_ebx(const char* op) {
    if (!op) return;
    if (is_number(op)) {
        long long v = is_float_literal(op) ? (long long)atof(op) : atoll(op);
        EMIT("    movl    $%lld, %%ebx\n", v);
    } else {
        const char* lbl = sym_lookup(op);
        if (lbl) EMIT("    movl    %s, %%ebx\n", lbl);
        else     EMIT("    movl    $0, %%ebx          # unknown: %s\n", op);
    }
}

static void store_from_eax(const char* name) {
    if (!name) return;
    const char* lbl = sym_lookup(name);
    if (!lbl) { sym_register(name, 0); lbl = sym_lookup(name); }
    EMIT("    movl    %%eax, %s\n", lbl);
}

/* -----------------------------------------------------------------------
 * Binary operator emission
 * ----------------------------------------------------------------------- */
static void emit_binop_asm(const char* op) {
    if (!op) return;
    if      (!strcmp(op,"+"))  { EMIT("    addl    %%ebx, %%eax\n"); }
    else if (!strcmp(op,"-"))  { EMIT("    subl    %%ebx, %%eax\n"); }
    else if (!strcmp(op,"*"))  { EMIT("    imull   %%ebx, %%eax\n"); }
    else if (!strcmp(op,"/"))  {
        EMIT("    movl    %%ebx, %%ecx\n");
        EMIT("    cdq\n");
        EMIT("    idivl   %%ecx\n");
    }
    else if (!strcmp(op,"<"))  { EMIT("    cmpl    %%ebx, %%eax\n"); EMIT("    setl    %%al\n"); EMIT("    movzbl  %%al, %%eax\n"); }
    else if (!strcmp(op,">"))  { EMIT("    cmpl    %%ebx, %%eax\n"); EMIT("    setg    %%al\n"); EMIT("    movzbl  %%al, %%eax\n"); }
    else if (!strcmp(op,"<=")) { EMIT("    cmpl    %%ebx, %%eax\n"); EMIT("    setle   %%al\n"); EMIT("    movzbl  %%al, %%eax\n"); }
    else if (!strcmp(op,">=")) { EMIT("    cmpl    %%ebx, %%eax\n"); EMIT("    setge   %%al\n"); EMIT("    movzbl  %%al, %%eax\n"); }
    else if (!strcmp(op,"==")) { EMIT("    cmpl    %%ebx, %%eax\n"); EMIT("    sete    %%al\n"); EMIT("    movzbl  %%al, %%eax\n"); }
    else if (!strcmp(op,"!=")) { EMIT("    cmpl    %%ebx, %%eax\n"); EMIT("    setne   %%al\n"); EMIT("    movzbl  %%al, %%eax\n"); }
    else if (!strcmp(op,"&&")) {
        EMIT("    testl   %%eax, %%eax\n");
        EMIT("    setne   %%al\n");
        EMIT("    movzbl  %%al, %%eax\n");
        EMIT("    testl   %%ebx, %%ebx\n");
        EMIT("    setne   %%bl\n");
        EMIT("    movzbl  %%bl, %%ebx\n");
        EMIT("    andl    %%ebx, %%eax\n");
    }
    else if (!strcmp(op,"||")) {
        EMIT("    testl   %%eax, %%eax\n");
        EMIT("    setne   %%al\n");
        EMIT("    movzbl  %%al, %%eax\n");
        EMIT("    testl   %%ebx, %%ebx\n");
        EMIT("    setne   %%bl\n");
        EMIT("    movzbl  %%bl, %%ebx\n");
        EMIT("    orl     %%ebx, %%eax\n");
    }
    else EMIT("    # unknown op: %s\n", op);
}

/* -----------------------------------------------------------------------
 * Track current function name
 * ----------------------------------------------------------------------- */
static char current_func[128] = "main";
static int  label_count = 0;

/* -----------------------------------------------------------------------
 * Main code generator
 * ----------------------------------------------------------------------- */
void generate_code(TACList* list) {
    prescan(list);

    fprintf(stdout, "\n--- X86 ASSEMBLY OUTPUT ---\n\n");

    /* ---- .data section ---- */
    EMIT(".section .data\n");
    EMIT("    fmt_int:   .string \"%%d\\n\"\n");
    EMIT("    fmt_char:  .string \"%%c\\n\"\n");
    EMIT("    fmt_float: .string \"%%f\\n\"\n");
    EMIT("    fmt_scan:  .string \"%%d\"\n");
    EMIT("    fmt_scanc: .string \" %%c\"\n");
    for (int i = 0; i < str_count; i++)
        EMIT("    %s: .string \"%s\\n\"\n",
             str_table[i].label, str_table[i].text);
    EMIT("\n");

    /* ---- .bss section ---- */
    EMIT(".section .bss\n");
    for (int i = 0; i < sym_count; i++)
        EMIT("    %s: .space 4\n", sym_table[i].asm_label);
    EMIT("\n");

    /* ---- .text section ---- */
    EMIT(".section .text\n");
    EMIT(".globl main\n");

    /* Check if there are any function definitions */
    int has_func = 0;
    for (TACInstr* c = list->head; c; c = c->next)
        if (c->op == TAC_FUNC_BEGIN) { has_func = 1; break; }

    /* No functions — emit main: prologue immediately */
    if (!has_func) {
        EMIT("main:\n");
        EMIT("    pushl   %%ebp\n");
        EMIT("    movl    %%esp, %%ebp\n");
        EMIT("\n");
    }

    /* Track whether we have emitted main: yet */
    int main_emitted = has_func ? 0 : 1;

    /* ----------------------------------------------------------------
     * Translate instructions
     * ---------------------------------------------------------------- */
    for (TACInstr* cur = list->head; cur; cur = cur->next) {

        switch (cur->op) {

            case TAC_FUNC_BEGIN:
                strncpy(current_func, cur->result, sizeof(current_func)-1);
                EMIT("\n%s:\n", cur->result);
                EMIT("    pushl   %%ebp\n");
                EMIT("    movl    %%esp, %%ebp\n");
                EMIT("\n");
                break;

            case TAC_FUNC_END:
                EMIT("    # end of function %s\n", cur->result);
                EMIT("    movl    $0, %%eax\n");
                EMIT("    movl    %%ebp, %%esp\n");
                EMIT("    popl    %%ebp\n");
                EMIT("    ret\n\n");
                /* After the last function definition, emit main: for
                 * the global statements that follow */
                if (!main_emitted) {
                    int next_is_func = (cur->next &&
                                        cur->next->op == TAC_FUNC_BEGIN);
                    if (!next_is_func) {
                        EMIT("main:\n");
                        EMIT("    pushl   %%ebp\n");
                        EMIT("    movl    %%esp, %%ebp\n");
                        EMIT("\n");
                        main_emitted = 1;
                    }
                }
                break;

            case TAC_DECLARE:
                EMIT("    # declare %s %s\n",
                     cur->arg1 ? cur->arg1 : "",
                     cur->result ? cur->result : "");
                break;

            case TAC_ASSIGN:
                EMIT("    # %s = %s\n",
                     cur->result ? cur->result : "",
                     cur->arg1   ? cur->arg1   : "");
                load_to_eax(cur->arg1);
                store_from_eax(cur->result);
                break;

            case TAC_BIN_OP:
                EMIT("    # %s = %s %s %s\n",
                     cur->result ? cur->result : "",
                     cur->arg1   ? cur->arg1   : "",
                     cur->op_str ? cur->op_str : "",
                     cur->arg2   ? cur->arg2   : "");
                load_to_eax(cur->arg1);
                load_to_ebx(cur->arg2);
                emit_binop_asm(cur->op_str);
                store_from_eax(cur->result);
                break;

            case TAC_LABEL:
                EMIT("%s:\n", cur->result ? cur->result : "");
                break;

            case TAC_GOTO:
                EMIT("    jmp     %s\n", cur->result ? cur->result : "");
                break;

            case TAC_IF_FALSE:
                EMIT("    # if_false %s goto %s\n",
                     cur->arg1 ? cur->arg1 : "",
                     cur->result ? cur->result : "");
                load_to_eax(cur->arg1);
                EMIT("    testl   %%eax, %%eax\n");
                EMIT("    je      %s\n", cur->result ? cur->result : "");
                break;

            case TAC_PRINT:
                if (cur->result && !cur->arg1) {
                    const char* slbl = str_register(cur->result);
                    EMIT("    # print \"%s\"\n", cur->result);
                    EMIT("    pushl   $%s\n", slbl);
                    EMIT("    call    printf\n");
                    EMIT("    addl    $4, %%esp\n");
                } else {
                    const char* varname = cur->arg1 ? cur->arg1 : "";
                    int use_char_fmt = sym_is_char(varname);
                    EMIT("    # print %s%s\n", varname,
                         use_char_fmt ? " (char)" : "");
                    load_to_eax(cur->arg1);
                    if (use_char_fmt) {
                        /* Runtime check: if value is printable ASCII (32-126)
                         * print as char, otherwise print as integer.
                         * This handles cases like d = 'Z' - 'A' = 25
                         * which is non-printable and should show as 25. */
                        EMIT("    cmpl    $32, %%eax\n");
                        EMIT("    jl      _print_as_int_%d\n", label_count);
                        EMIT("    cmpl    $126, %%eax\n");
                        EMIT("    jg      _print_as_int_%d\n", label_count);
                        EMIT("    pushl   %%eax\n");
                        EMIT("    pushl   $fmt_char\n");
                        EMIT("    call    printf\n");
                        EMIT("    addl    $8, %%esp\n");
                        EMIT("    jmp     _print_done_%d\n", label_count);
                        EMIT("_print_as_int_%d:\n", label_count);
                        EMIT("    pushl   %%eax\n");
                        EMIT("    pushl   $fmt_int\n");
                        EMIT("    call    printf\n");
                        EMIT("    addl    $8, %%esp\n");
                        EMIT("_print_done_%d:\n", label_count);
                        label_count++;
                    } else {
                        EMIT("    pushl   %%eax\n");
                        EMIT("    pushl   $fmt_int\n");
                        EMIT("    call    printf\n");
                        EMIT("    addl    $8, %%esp\n");
                    }
                }
                break;

            case TAC_SCAN: {
                EMIT("    # scan %s\n", cur->result ? cur->result : "");
                const char* lbl = sym_lookup(cur->result);
                if (!lbl) {
                    sym_register(cur->result, 0);
                    lbl = sym_lookup(cur->result);
                }
                int scan_char = sym_is_char(cur->result);
                EMIT("    pushl   $%s\n", lbl);
                EMIT("    pushl   $%s\n", scan_char ? "fmt_scanc" : "fmt_scan");
                EMIT("    call    scanf\n");
                EMIT("    addl    $8, %%esp\n");
                break;
            }

            case TAC_RETURN:
                EMIT("    # return %s\n", cur->arg1 ? cur->arg1 : "");
                if (cur->arg1) load_to_eax(cur->arg1);
                else           EMIT("    movl    $0, %%eax\n");
                EMIT("    movl    %%ebp, %%esp\n");
                EMIT("    popl    %%ebp\n");
                EMIT("    ret\n");
                break;

            case TAC_PARAM:
                EMIT("    # param %s\n", cur->arg1 ? cur->arg1 : "");
                load_to_eax(cur->arg1);
                EMIT("    pushl   %%eax\n");
                break;

            case TAC_CALL: {
                int argc = cur->arg2 ? atoi(cur->arg2) : 0;
                EMIT("    # %s = call %s %d\n",
                     cur->result ? cur->result : "_",
                     cur->arg1   ? cur->arg1   : "?",
                     argc);
                EMIT("    call    %s\n", cur->arg1 ? cur->arg1 : "");
                if (argc > 0)
                    EMIT("    addl    $%d, %%esp\n", argc * 4);
                if (cur->result) store_from_eax(cur->result);
                break;
            }
        }

        EMIT("\n");
    }

    /* Final epilogue for main */
    EMIT("    movl    $0, %%eax\n");
    EMIT("    movl    %%ebp, %%esp\n");
    EMIT("    popl    %%ebp\n");
    EMIT("    ret\n");
    fprintf(stdout, "\n---------------------------\n");
}
