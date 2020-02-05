#include "ast.h"
#include "memm.h"
#include "statement.h"
#include "expr.h"
#include "reference.h"
#include "flow.h"
#include "europa_debug.h"
#include "europa_error.h"


struct e_stmt *stmt_factory(){
    return (struct e_stmt *)memm_alloc(sizeof(struct e_stmt));    
}

struct e_stmt *stmt_create_expr(struct ast_node *n){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = s_expr;	
    newstmt->expr = n;    
    return newstmt;    
}

struct e_stmt *stmt_create_flow(unsigned int type, struct e_flow *flow){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = type;    
    newstmt->flow = flow; 
    return newstmt;
}

struct e_stmt *stmt_create_assign(struct ast_node *n){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = s_assign;
    newstmt->expr = n; 
    return newstmt;
}

struct e_stmt *stmt_create_func_def(struct e_reference *func_def){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = s_func_def;
    newstmt->func_def = func_def; 
    return newstmt;    
}

struct e_stmt *stmt_create_return(struct ast_node *n){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = s_return;	
    newstmt->expr = n;    
    return newstmt;    
}


void stmt_eval(struct e_stmt *stmt){    
    switch(stmt->type){
        case s_assign: 
            // Assignment eval			 
            DEBUG_OUTPUT("ASSIGNMENT_EVAL\n");
            assignment_eval(stmt->expr->assign->reference, stmt->expr->assign->expr);
            break;
        case s_expr: 
            // Expression statement            	
            DEBUG_OUTPUT("EXPR_EVAL");
            value_eval(expr_eval(stmt->expr));				            
            break;
        case s_if_flow: 
            // if statement eval
            DEBUG_OUTPUT("IF_EVAL");
            if_stmt_eval(stmt->flow, NULL);
            break;
        case s_while_flow: 
            // While statement eval
            DEBUG_OUTPUT("WHILE_EVAL");
            while_stmt_eval(stmt->flow, NULL);
            break;        
		break;
        default:
            EUROPA_ERROR("Unable to evaluate statement of type '%i'\n", stmt->type);            
    }
}
