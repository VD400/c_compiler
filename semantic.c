#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

/* Forward declaration */
char* analyze_semantics(ASTNode* node);

/* -----------------------------------------------------------------------
 * prescan_functions — PASS 1
 *
 * Walk only the top-level node list and register every function
 * definition into the function table BEFORE the main analysis runs.
 * This means functions can be called before they are defined in the
 * source file (standard C behaviour).
 *
 * This is called from main.c before analyze_semantics().
 * ----------------------------------------------------------------------- */
void prescan_functions(ASTNode* node) {
    ASTNode* cur = node;
    while (cur) {
        if (cur->type == NODE_FUNC_DEF) {
            /* Build the parameter info list */
            ParamInfo* param_list = NULL;
            ParamInfo* param_tail = NULL;
            int param_count = 0;

            ASTNode* p = cur->params;
            while (p) {
                ParamInfo* pi  = (ParamInfo*)malloc(sizeof(ParamInfo));
                pi->name       = strdup(p->value);
                pi->data_type  = strdup(p->data_type);
                pi->next       = NULL;
                if (!param_list) { param_list = param_tail = pi; }
                else             { param_tail->next = pi; param_tail = pi; }
                param_count++;
                p = p->next;
            }

            add_function(cur->value, cur->data_type, param_list, param_count);
        }
        cur = cur->next;
    }
}

/* -----------------------------------------------------------------------
 * analyze_semantics — PASS 2
 *
 * Full recursive semantic analysis. By the time this runs, all function
 * signatures are already in the function table from prescan_functions().
 * ----------------------------------------------------------------------- */
char* analyze_semantics(ASTNode* node) {
    if (!node) return NULL;

    char* return_type = NULL;

    switch (node->type) {

        /* ---- Variable declaration ---- */
        case NODE_VAR_DECL:
            add_symbol(node->value, node->data_type);
            return_type = node->data_type;
            break;

        /* ---- Identifier use ---- */
        case NODE_ID: {
            Symbol* sym = lookup_symbol(node->value);
            if (!sym) {
                fprintf(stderr,
                    "Semantic Error: Variable '%s' not declared before use.\n",
                    node->value);
                exit(1);
            }
            return_type = sym->data_type;
            break;
        }

        /* ---- Literals ---- */
        case NODE_INT:   return_type = "int";    break;
        case NODE_FLOAT: return_type = "float";  break;
        case NODE_CHAR:  return_type = "char";   break;
        case NODE_STR:   return_type = "string"; break;

        /* ---- Assignment ---- */
        case NODE_ASSIGN: {
            Symbol* sym = lookup_symbol(node->left->value);
            if (!sym) {
                fprintf(stderr,
                    "Semantic Error: Assignment to undeclared variable '%s'\n",
                    node->left->value);
                exit(1);
            }
            char* rtype = analyze_semantics(node->right);
            /* Allow int <-> char assignment silently (both are integers at runtime) */
            int compatible = 0;
            if (!rtype) compatible = 1;
            else if (strcmp(sym->data_type, rtype) == 0) compatible = 1;
            else if ((strcmp(sym->data_type,"char")==0 && strcmp(rtype,"int")==0)) compatible = 1;
            else if ((strcmp(sym->data_type,"int")==0  && strcmp(rtype,"char")==0)) compatible = 1;
            if (!compatible)
                fprintf(stderr,
                    "Warning: Type mismatch — assigning '%s' to '%s' variable '%s'\n",
                    rtype, sym->data_type, sym->name);
            return_type = sym->data_type;
            break;
        }

        /* ---- Binary operation ---- */
        case NODE_BIN_OP: {
            char* lt = analyze_semantics(node->left);
            char* rt = analyze_semantics(node->right);
            if (lt && rt)
                return_type = (strcmp(lt,"float")==0 || strcmp(rt,"float")==0)
                              ? "float" : "int";
            break;
        }

        /* ---- Block: new scope ---- */
        case NODE_BLOCK:
            push_scope();
            analyze_semantics(node->left);
            pop_scope();
            break;

        /* ---- If / while ---- */
        case NODE_IF:
            analyze_semantics(node->condition);
            analyze_semantics(node->left);
            if (node->else_body) analyze_semantics(node->else_body);
            break;

        case NODE_WHILE:
            analyze_semantics(node->condition);
            analyze_semantics(node->left);
            break;

        /* ---- I/O and jumps ---- */
        case NODE_PRINT:
        case NODE_SCAN:
        case NODE_RETURN:
            analyze_semantics(node->left);
            break;

        case NODE_BREAK:
        case NODE_CONTINUE:
            break;

        /* ---- Function definition ---- */
        /* prescan_functions() already registered this function.
         * Here we only need to analyse the body in a new scope
         * with the parameters added as local variables.          */
        case NODE_FUNC_DEF: {
            push_scope();
            /* Add each parameter as a local variable */
            ASTNode* p = node->params;
            while (p) {
                add_symbol(p->value, p->data_type);
                p = p->next;
            }
            /* Analyse the function body */
            analyze_semantics(node->left);
            pop_scope();
            break;
        }

        /* ---- Function call ---- */
        case NODE_FUNC_CALL: {
            FuncSymbol* func = lookup_function(node->value);
            if (!func) {
                fprintf(stderr,
                    "Semantic Error: Call to undefined function '%s'\n",
                    node->value);
                exit(1);
            }

            /* Count arguments */
            int argc = 0;
            ASTNode* arg = node->args;
            while (arg) { argc++; arg = arg->next; }

            if (argc != func->param_count) {
                fprintf(stderr,
                    "Semantic Error: Function '%s' expects %d argument(s), got %d\n",
                    node->value, func->param_count, argc);
                exit(1);
            }

            /* Analyse each argument expression */
            arg = node->args;
            while (arg) {
                analyze_semantics(arg);
                arg = arg->next;
            }

            return_type = func->return_type;
            break;
        }

        default:
            analyze_semantics(node->left);
            analyze_semantics(node->right);
            break;
    }

    /* Move to the next statement in the current block */
    if (node->next) analyze_semantics(node->next);

    return return_type;
}
