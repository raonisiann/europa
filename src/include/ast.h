#ifndef I_AST_H
#define I_AST_H
#include "lexer.h"
#include "europa_types.h"

struct ast_node {
    struct ast_node *left; 
    struct ast_node *right;
    struct lex_token *token;
};

struct ast_assignment_node {
    struct ast_node *reference;
    struct ast_node *assigment;
};

struct ast_node* ast_add_node(struct lex_token *t, struct ast_node *l, struct ast_node *r);
struct ast_node* ast_factory_node();
struct ast_assignment_node *ast_factory_assignment_node();
struct ast_assignment_node* ast_create_assignment(struct ast_node *ref, struct ast_node *assign);
void ast_free(struct ast_node *node);



#endif