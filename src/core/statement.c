
//#include "sd_types.h"
#include "ast.h"
//#include "sd_debug.h"
//#include "branchs.h"
#include "memm.h"
#include "statement.h"
#include "expr.h"
#include "reference.h"
#include "flow.h"
#include "europa_debug.h"
#include "europa_error.h"
//#include "variables.h"
//#include "sd_lex.h"
//#include "loops.h"
//#include "types_operations.h"


struct e_stmt *stmt_factory(){
    return (struct e_stmt *)memm_alloc(sizeof(struct e_stmt));    
}

struct e_stmt *stmt_create_expr(struct ast_node *n){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = expression;	
    newstmt->expr = n;    
    return newstmt;    
}

struct e_stmt *stmt_create_flow(unsigned int type, struct e_flow *flow){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = type;    
    newstmt->flow = flow; 
    return newstmt;
}

struct e_stmt *stmt_create_assign(struct e_assignment *assignment){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = assign;
    newstmt->assign = assignment; 
    return newstmt;
}


void stmt_eval(struct e_stmt *stmt){    
    switch(stmt->type){
        case 'C': 
			//DEBUG_OUTPUT("Starting eval of an C\n");		
			//ast_eval(stmt->data.ast, c);
            break;
        case assign: // Assignment eval			 
            DEBUG_OUTPUT("ASSIGNMENT_EVAL");
            assignment_eval(stmt->assign);
            break;
        case expression: // Expression statement            	
            DEBUG_OUTPUT("EXPR_EVAL");
            value_eval(expr_eval(stmt->expr));				            
            break;
        case ifcmd: 
            // if statement eval
            DEBUG_OUTPUT("IF_EVAL");
            if_stmt_eval(stmt->flow, NULL);
            break;
        case whilecmd: 
            // While statement eval
            DEBUG_OUTPUT("WHILE_EVAL");
            while_stmt_eval(stmt->flow, NULL);
            break;        
		case 'R':			
			//c->retval = ast_eval(stmt->data.ast, c);
			//DEBUG_OUTPUT("RETURN VAL type '%c'\n", c->retval->type);
            break;
		break;
        default:
            EUROPA_ERROR("Unable to evaluate statement of type %c\n", stmt->type);            
    }
}
