#ifndef I_FLOW_H
#define I_FLOW_H

#include "list.h"
#include "ast.h"
#include "context.h"

struct e_flow {    
    struct ast_node *cond;
    struct list *if_true;    
    struct list *if_false;    
};

struct e_flow *flow_factory();
struct e_flow *flow_create(struct ast_node *cond, struct list *if_true, struct list *if_false);
void if_stmt_eval(struct e_flow *flow, struct e_context *ctxt);
void while_stmt_eval(struct e_flow *flow, struct e_context *ctxt);

#endif