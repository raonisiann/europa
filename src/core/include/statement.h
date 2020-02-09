#ifndef I_STATEMENT_H
#define I_STATEMENT_H

#include "europa_types.h"
#include "context.h"

typedef enum {
    s_expr = 1, 
    s_if_flow, 
    s_while_flow,
    s_assign, 
    s_func_def, 
    s_return 
} stmt_type;

struct e_stmt {
    unsigned int type;
    union {
        struct ast_node *expr;  
        struct e_assignment *assign;
        struct e_flow *flow;   
        struct e_reference *func_def;  
        //struct dl_list *cmdlist;        
        //struct func_def *function;         
    }; 
};


struct e_stmt *stmt_factory();
void stmt_eval(struct e_stmt *stmt, struct e_context *ctxt);
void stmt_block_eval(struct list *stmt_list, struct e_context *ctxt);
struct e_stmt *stmt_create_expr(struct ast_node *n);
struct e_stmt *stmt_create_flow(unsigned int type, struct e_flow *flow);
struct e_stmt *stmt_create_assign(struct ast_node *n);
struct e_stmt *stmt_create_func_def(struct e_reference *func_def);
struct e_stmt *stmt_create_return(struct ast_node *n);

#endif