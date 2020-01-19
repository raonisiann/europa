#ifndef I_STATEMENT_H
#define I_STATEMENT_H

#include "europa_types.h"

struct e_stmt {
    unsigned int type;
    union {
        struct ast_node *expr;  
        struct ast_assignment_node *assign;
        //struct dl_list *cmdlist;
        //struct lflow *flow;    
        //struct func_def *function;         
    }; 
};


struct e_stmt *stmt_factory();
void stmt_eval(struct e_stmt *stmt);
struct e_stmt *stmt_create_expr(struct ast_node *n);
struct e_stmt *stmt_create_assign(struct ast_assignment_node *a);

#endif