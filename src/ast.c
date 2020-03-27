#include <stdlib.h>
#include <ctype.h>
#include "ast.h"
#include "lexer.h"
#include "europa_types.h"
#include "memm.h"

// append node to tree
struct ast_node* ast_add_node(struct lex_token *t, struct ast_node *l, struct ast_node *r){
    struct ast_node* n = ast_factory_node(); 
    n->left = l;
    n->right = r;
    n->token = t;
    return n;
}

struct ast_node *ast_add_token_node(struct lex_token *t, struct ast_node *l, struct ast_node *r){
    struct ast_node* n = ast_factory_node(); 
    n->type = node_token;
    n->left = l;
    n->right = r;
    n->token = t;
    return n;    
}

struct ast_node *ast_add_value_node(struct e_value *val, struct ast_node *l, struct ast_node *r){
    struct ast_node* n = ast_factory_node(); 
    n->type = node_value;
    n->left = l;
    n->right = r;
    n->val = val;
    return n;   
}

struct ast_node *ast_add_fcall_node(struct lex_token *fcall_tk, struct list *args){
    struct ast_node* n = ast_factory_node(); 
    n->type = node_fcall;
    n->fcall = ast_factory_fcall_node();
    n->fcall->func = fcall_tk;
    n->fcall->expr_list = args;
    return n;  
}

struct ast_node *ast_add_assign_node(struct lex_token *ref_tk, struct ast_node *expr){
    struct ast_node *n = ast_factory_node();
    n->type = node_assign; 
    n->assign = ast_factory_assign_node();
    n->assign->reference = ref_tk;
    n->assign->expr = expr; 
    return n;
}

// create ast node 
struct ast_node *ast_factory_node(){
    struct ast_node *new_node = ((struct ast_node *)memm_alloc(sizeof(struct ast_node)));
    new_node->type = node_undefined;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->val = NULL;
    return new_node;
}

// create an ast assignment node 
struct ast_assignment_node *ast_factory_assignment_node(){
    return ((struct ast_assignment_node *)memm_alloc(sizeof(struct ast_assignment_node)));
}

struct ast_node_fcall *ast_factory_fcall_node(){
    struct ast_node_fcall *fcall = (struct ast_node_fcall *)memm_alloc(sizeof(struct ast_node_fcall));
    fcall->func = NULL;
    fcall->expr_list = NULL;    
    return fcall; 
}

struct ast_node_assign *ast_factory_assign_node(){
    struct ast_node_assign *assign = (struct ast_node_assign *)memm_alloc(sizeof(struct ast_node_assign));
    assign->reference = NULL;
    assign->expr = NULL;
    return assign;
}

// free node tree
void ast_free(struct ast_node *node){
    if(node->left != NULL){
        ast_free(node->left);               
    }
    if(node->right != NULL){
        ast_free(node->right);
    }
    // free up the leaf nodes
    memm_free(node);
}
