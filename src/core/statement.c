
//#include "sd_types.h"
#include "ast.h"
//#include "sd_debug.h"
//#include "branchs.h"
#include "memm.h"
#include "statement.h"
#include "expr.h"
#include "reference.h"
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

struct e_stmt *stmt_create_assign(struct ast_assignment_node *a){
    struct e_stmt *newstmt = stmt_factory();
    newstmt->type = assign;
    newstmt->assign = a; 
}


void stmt_eval(struct e_stmt *stmt){    
    switch(stmt->type){
        case 'C': 
			//DEBUG_OUTPUT("Starting eval of an C\n");		
			//ast_eval(stmt->data.ast, c);
            break;
        case assign: // Assignment eval			
			//DEBUG_OUTPUT("Starting eval of an ASSINGMENT\n");
			//assignment_eval(stmt->data.ast, c);            
            printf("ASSIGNMENT_EVAL  :: \n");
            assignment_eval(stmt->assign);
            break;
        case expression: // Expression statement            
			//DEBUG_OUTPUT("Starting eval of an EXPR\n");
            //expr_eval(ast_eval(stmt->data.ast, c));	
            printf("EXPR_EVAL        :: \n");
            value_eval(expr_eval(stmt->expr));				            
            break;
        case 'F': // Flow statement (if, else if, while...)       
            /*     
            switch(stmt->data.flow->type){
                case 'I':
					DEBUG_OUTPUT("Starting eval of an IF\n");
                    ifstmt_eval(stmt->data.flow, c);
                break;
                case 'W': // while
					DEBUG_OUTPUT("Starting eval of an WHILE\n");
                    whilestmt_eval(stmt->data.flow, c);
                break;                
                default:
                    yyerror("Unknown flow type %c\n", stmt->data.flow->type);
            }
            */
            break;  
		case 'R':			
			//c->retval = ast_eval(stmt->data.ast, c);
			//DEBUG_OUTPUT("RETURN VAL type '%c'\n", c->retval->type);
            break;
		break;
        default:
            printf("Unable to evaluate statement of type %c\n", stmt->type);            
    }
}
