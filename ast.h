#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef enum {
    NODE_INT,
    NODE_FLOAT,
    NODE_CHAR,
    NODE_STR,
    NODE_ID,
    NODE_VAR_DECL,
    NODE_BIN_OP,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_PRINT,
    NODE_SCAN,
    NODE_STMT_LST,
    NODE_RETURN,
    NODE_BREAK,
    NODE_CONTINUE,
    NODE_BLOCK,
    NODE_REL_OP,
    NODE_LOGIC_OP,
    /* ---- NEW: function support ---- */
    NODE_FUNC_DEF,   /* int add(int a, int b) { ... }          */
    NODE_FUNC_CALL,  /* result = add(x, y)                     */
    NODE_PARAM       /* a single parameter in a param list      */
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* value;               /* name, operator, literal text          */
    char* data_type;           /* "int" / "float" / "char" for decls    */
    struct ASTNode *left;      /* true-body / LHS / func body           */
    struct ASTNode *right;     /* RHS                                   */
    struct ASTNode *condition; /* if/while condition                    */
    struct ASTNode *else_body; /* else branch                           */
    struct ASTNode *next;      /* next statement in a list              */
    /* ---- NEW fields for functions ---- */
    struct ASTNode *params;    /* linked list of NODE_PARAM nodes       */
    struct ASTNode *args;      /* linked list of arg expressions        */
} ASTNode;

/* Existing constructors */
ASTNode* create_node(NodeType type, char* value, ASTNode* left, ASTNode* right);
ASTNode* create_if_node(ASTNode* cond, ASTNode* body, ASTNode* else_b);
ASTNode* create_while_node(ASTNode* cond, ASTNode* body);
ASTNode* create_var_node(char* data_type, char* var_name);
ASTNode* create_io_node(NodeType type, ASTNode* target);
ASTNode* create_jump_node(NodeType type, ASTNode* return_value);
ASTNode* create_block_node(ASTNode* stmt_list);

/* NEW constructors for function support */
ASTNode* create_param_node(char* data_type, char* name);
ASTNode* create_func_def_node(char* return_type, char* name,
                               ASTNode* params, ASTNode* body);
ASTNode* create_func_call_node(char* name, ASTNode* args);

void print_ast(ASTNode* node, int level);

#endif
