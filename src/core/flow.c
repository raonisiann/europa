#include <stdio.h>
#include "europa_debug.h"
#include "ast.h"
#include "memm.h"
#include "europa_types.h"
#include "flow.h"
#include "statement.h"
#include "list.h"
#include "lexer.h"
#include "expr.h"
#include "europa_io.h"
#include "europa_error.h"


void if_stmt_eval(struct e_flow *flow, struct e_context *ctxt){   
    struct e_value *condres = expr_eval(flow->cond); 
    if(condres->type != e_boolean){
        EUROPA_ERROR("If condition expression must to result in a boolean value");        
    }        
    if(condres->boolean == e_true){       
        if(flow->if_true->size > 0){
            stmt_block_eval(flow->if_true, ctxt);
        }
    }else if(condres->boolean == e_false && flow->if_false != NULL){        
        if(flow->if_false->size > 0){            
            stmt_block_eval(flow->if_false, ctxt);                                 
        }        
    }
}


void while_stmt_eval(struct e_flow *flow, struct e_context *ctxt){   
    struct e_value *condres = expr_eval(flow->cond);    
    if(condres->type != e_boolean){
        EUROPA_ERROR("While expression must to result in a boolean value");        
    }      	
    if(flow->if_true->size > 0){
		DEBUG_OUTPUT("WHILE if TRUE branch greater than zero");
        while(condres->boolean == e_true){            			
			DEBUG_OUTPUT("WHILE condition result TRUE\n");	            
            stmt_block_eval(flow->if_true, ctxt);
            condres = expr_eval(flow->cond);        
        }
    }
}


struct e_flow *flow_factory(){
    struct e_flow *new_flow = (struct e_flow *)memm_alloc(sizeof(struct e_flow)); 
    new_flow->cond = NULL;
    new_flow->if_true = NULL;
    new_flow->if_false = NULL;
    return new_flow;
}

// Create a new language flow 
// Will be used for if, if else, while and other flow structures...
struct e_flow *flow_create(struct ast_node *cond, struct list *if_true, struct list *if_false){    
    struct e_flow *flow = flow_factory();        
    flow->cond = cond;
    flow->if_true = if_true;
    flow->if_false = if_false;
	return flow;
}
