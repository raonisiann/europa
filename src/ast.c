#include <stdlib.h>
#include <ctype.h>
#include "ast.h"
#include "lexer.h"
#include "europa_types.h"
#include "memm.h"

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

// Create assignment node 
struct ast_assignment_node* ast_create_assignment(struct ast_node *ref, struct ast_node *assign){
    struct ast_assignment_node* n = ast_factory_assignment_node(); 
    n->reference = ref;
    n->assigment = assign;
    return n;
}

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

struct ast_node *ast_add_reference_node(struct e_reference *ref, struct ast_node *l, struct ast_node *r){
    struct ast_node* n = ast_factory_node(); 
    n->type = node_reference;
    n->left = l;
    n->right = r;
    n->ref = ref;
    return n;       
}

// free node 
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
