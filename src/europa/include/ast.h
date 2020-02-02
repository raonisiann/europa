#ifndef I_AST_H
#define I_AST_H
#include "lexer.h"
#include "europa_types.h"


typedef enum {
    node_undefined = -1,
    node_token = 1, 
    node_value,
    node_reference, 
    node_fcall, 
    node_assign
} ast_node_type;


struct ast_node {
    char type; 
    struct ast_node *left; 
    struct ast_node *right;    
    union {
        struct lex_token *token;
        struct e_value *val;
        struct ast_node_fcall *fcall; 
        struct ast_node_assign *assign;
    };
};

struct ast_node_fcall {
    struct lex_token *func;
    struct list *expr_list; 
};

struct ast_node_assign {
    struct lex_token *reference;
    struct ast_node *expr;
};

struct ast_assignment_node {
    struct ast_node *reference;
    struct ast_node *assigment;
};

struct ast_node *ast_add_node(struct lex_token *t, struct ast_node *l, struct ast_node *r);

struct ast_node *ast_add_token_node(struct lex_token *t, struct ast_node *l, struct ast_node *r);
struct ast_node *ast_add_value_node(struct e_value *val, struct ast_node *l, struct ast_node *r);
struct ast_node *ast_add_fcall_node(struct lex_token *fcall_tk, struct list *args);
struct ast_node *ast_add_assign_node(struct lex_token *ref_tk, struct ast_node *expr);

struct ast_node *ast_factory_node();
struct ast_node_fcall *ast_factory_fcall_node();
struct ast_node_assign *ast_factory_assign_node();
struct ast_assignment_node *ast_factory_assignment_node();
struct ast_assignment_node *ast_create_assignment(struct ast_node *ref, struct ast_node *assign);
void ast_free(struct ast_node *node);



#endif