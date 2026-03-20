/* ast_func.c — add these functions to your existing ast.c
 *
 * INSTRUCTIONS:
 *   Copy the three constructor functions below into ast.c (before the
 *   closing #endif or at the end of the file).
 *   Then add the two new case blocks into print_ast_node()'s switch.
 *
 * OR simply replace ast.c entirely with this file — it includes all the
 * original functions plus the new ones.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern FILE* ast_file;
#define PRINT_AST(...) do { fprintf(stdout, __VA_ARGS__); fprintf(ast_file, __VA_ARGS__); } while(0)

/* -----------------------------------------------------------------------
 * Original constructors (unchanged)
 * ----------------------------------------------------------------------- */
ASTNode* create_node(NodeType type, char* value, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) { fprintf(stderr,"Error: malloc failed\n"); exit(1); }
    node->type      = type;
    node->value     = value ? strdup(value) : NULL;
    node->data_type = NULL;
    node->left      = left;
    node->right     = right;
    node->condition = NULL;
    node->else_body = NULL;
    node->next      = NULL;
    node->params    = NULL;
    node->args      = NULL;
    return node;
}

ASTNode* create_if_node(ASTNode* cond, ASTNode* body, ASTNode* else_b) {
    ASTNode* node   = create_node(NODE_IF, NULL, body, NULL);
    node->condition = cond;
    node->else_body = else_b;
    return node;
}

ASTNode* create_while_node(ASTNode* cond, ASTNode* body) {
    ASTNode* node   = create_node(NODE_WHILE, NULL, body, NULL);
    node->condition = cond;
    return node;
}

ASTNode* create_io_node(NodeType type, ASTNode* target) {
    return create_node(type, NULL, target, NULL);
}

ASTNode* create_var_node(char* data_type, char* var_name) {
    ASTNode* node   = create_node(NODE_VAR_DECL, var_name, NULL, NULL);
    node->data_type = strdup(data_type);
    return node;
}

ASTNode* create_jump_node(NodeType type, ASTNode* return_value) {
    return create_node(type, NULL, return_value, NULL);
}

ASTNode* create_block_node(ASTNode* stmt_list) {
    return create_node(NODE_BLOCK, NULL, stmt_list, NULL);
}

/* -----------------------------------------------------------------------
 * NEW constructors for function support
 * ----------------------------------------------------------------------- */

/* A single parameter:  int a  →  NODE_PARAM, value="a", data_type="int" */
ASTNode* create_param_node(char* data_type, char* name) {
    ASTNode* node   = create_node(NODE_PARAM, name, NULL, NULL);
    node->data_type = strdup(data_type);
    return node;
}

/* Function definition:  int add(int a, int b) { body }
 *   value      = "add"
 *   data_type  = "int"   (return type)
 *   params     = linked list of NODE_PARAM nodes
 *   left       = body block
 */
ASTNode* create_func_def_node(char* return_type, char* name,
                               ASTNode* params, ASTNode* body) {
    ASTNode* node   = create_node(NODE_FUNC_DEF, name, body, NULL);
    node->data_type = strdup(return_type);
    node->params    = params;
    return node;
}

/* Function call:  add(x, y)
 *   value = "add"
 *   args  = linked list of argument expression nodes
 */
ASTNode* create_func_call_node(char* name, ASTNode* args) {
    ASTNode* node = create_node(NODE_FUNC_CALL, name, NULL, NULL);
    node->args    = args;
    return node;
}

/* -----------------------------------------------------------------------
 * Printer helpers (same as original, copied here for completeness)
 * ----------------------------------------------------------------------- */
static void print_ast_node(ASTNode* node, const char* prefix, int is_last);

static void print_children(ASTNode* list, const char* prefix, int last_sibling) {
    ASTNode* cur = list;
    while (cur) {
        int is_last = (cur->next == NULL) && last_sibling;
        print_ast_node(cur, prefix, is_last);
        cur = cur->next;
    }
}

static void print_ast_node(ASTNode* node, const char* prefix, int is_last) {
    if (!node) return;

    PRINT_AST("%s%s", prefix, is_last ? "└── " : "├── ");

    char new_prefix[2048];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix,
             is_last ? "    " : "│   ");

    switch (node->type) {

        case NODE_VAR_DECL:
            PRINT_AST("DECLARATION %s %s\n",
                      node->data_type ? node->data_type : "?", node->value);
            break;

        case NODE_ASSIGN:
            PRINT_AST("ASSIGN\n");
            print_ast_node(node->left,  new_prefix, 0);
            print_ast_node(node->right, new_prefix, 1);
            break;

        case NODE_BIN_OP:
	case NODE_LOGIC_OP:
	case NODE_REL_OP:
    	    PRINT_AST("(%s)\n", node->value);
            print_ast_node(node->left,  new_prefix, 0);
            if (node->right) print_ast_node(node->right, new_prefix, 1);
            break;

        case NODE_ID:
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_STR:
            PRINT_AST("%s\n", node->value ? node->value : "");
            break;

        case NODE_PRINT:
            PRINT_AST("PRINT\n");
            print_ast_node(node->left, new_prefix, 1);
            break;

        case NODE_SCAN:
            PRINT_AST("SCAN\n");
            print_ast_node(node->left, new_prefix, 1);
            break;

        case NODE_RETURN:
            PRINT_AST("RETURN\n");
            if (node->left) print_ast_node(node->left, new_prefix, 1);
            break;

        case NODE_BREAK:    PRINT_AST("BREAK\n");    break;
        case NODE_CONTINUE: PRINT_AST("CONTINUE\n"); break;

        case NODE_IF: {
            PRINT_AST("IF\n");
            PRINT_AST("%s├── CONDITION\n", new_prefix);
            char cond_prefix[2048];
            snprintf(cond_prefix, sizeof(cond_prefix), "%s│   ", new_prefix);
            print_ast_node(node->condition, cond_prefix, 1);
            int has_else = (node->else_body != NULL);
            if (node->left) print_children(node->left, new_prefix, !has_else);
            if (has_else) {
                PRINT_AST("%s└── ELSE\n", new_prefix);
                char ep[2048];
                snprintf(ep, sizeof(ep), "%s    ", new_prefix);
                print_children(node->else_body, ep, 1);
            }
            break;
        }

        case NODE_WHILE:
            PRINT_AST("WHILE\n");
            PRINT_AST("%s├── CONDITION\n", new_prefix);
            char wcp[2048];
            snprintf(wcp, sizeof(wcp), "%s│   ", new_prefix);
            print_ast_node(node->condition, wcp, 1);
            if (node->left) print_children(node->left, new_prefix, 1);
            break;

        case NODE_BLOCK:
            PRINT_AST("BLOCK\n");
            if (node->left) print_children(node->left, new_prefix, 1);
            break;

        /* ---- NEW: function definition ---- */
        case NODE_FUNC_DEF:
            PRINT_AST("FUNC_DEF %s %s\n",
                      node->data_type ? node->data_type : "?", node->value);
            /* Print parameters */
            if (node->params) {
                PRINT_AST("%s├── PARAMS\n", new_prefix);
                char pp[2048];
                snprintf(pp, sizeof(pp), "%s│   ", new_prefix);
                ASTNode* p = node->params;
                while (p) {
                    int plast = (p->next == NULL);
                    PRINT_AST("%s%s%s %s\n", pp,
                              plast ? "└── " : "├── ",
                              p->data_type ? p->data_type : "?",
                              p->value ? p->value : "?");
                    p = p->next;
                }
            }
            /* Print body */
            if (node->left) print_ast_node(node->left, new_prefix, 1);
            break;

        /* ---- NEW: function call ---- */
        case NODE_FUNC_CALL:
            PRINT_AST("FUNC_CALL %s\n", node->value ? node->value : "?");
            if (node->args) {
                PRINT_AST("%s└── ARGS\n", new_prefix);
                char ap[2048];
                snprintf(ap, sizeof(ap), "%s    ", new_prefix);
                ASTNode* a = node->args;
                while (a) {
                    int alast = (a->next == NULL);
                    print_ast_node(a, ap, alast);
                    a = a->next;
                }
            }
            break;

        case NODE_PARAM:
            PRINT_AST("PARAM %s %s\n",
                      node->data_type ? node->data_type : "?",
                      node->value     ? node->value     : "?");
            break;

        default:
            PRINT_AST("UNKNOWN(%d)\n", node->type);
            break;
    }
}

void print_ast(ASTNode* node, int level) {
    if (!node) return;
    PRINT_AST("PROGRAM\n");
    ASTNode* cur = node;
    while (cur) {
        print_ast_node(cur, "", cur->next == NULL);
        cur = cur->next;
    }
}
