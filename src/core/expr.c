#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memm.h"
#include "expr.h"
#include "europa_types.h"
#include "europa_debug.h"
#include "lexer.h"
#include "reference.h"
#include "europa_io.h"
#include "europa_error.h"
#include "stack.h"


struct e_value *do_arithmetic_sum(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();
	if(l->type == e_string || r->type == e_string){
		// this is considered an string concatenation
		res->str = do_string_concat(l, r);
		res->type = e_string;
        res->size = l->size + r->size;
	}else{
		res->num = l->num + r->num;
		res->type = e_int;
	}    
    return res;
}

struct e_value *do_arithmetic_subtract(struct e_value *l, struct e_value *r){   
    struct e_value *res = factory_value();
    res->num = l->num - r->num;
    res->type = e_int;
    return res;
}

struct e_value *do_arithmetic_multiply(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();
    res->num = l->num * r->num;
    res->type = e_int;
    return res;
}

struct e_value *do_arithmetic_division(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();
    if(l->type != e_int || r->type != e_int){
        EUROPA_ERROR("Arithmetic division is only supported by numbers. Not '%c' with '%c'\n", l->type, r->type);          
    }    
    res->num = (l->num / r->num);
    res->type = e_int;
    return res;
}

// Do the 'equal' logic
struct e_value *do_logical_equal(struct e_value *l, struct e_value *r){   
    struct e_value *res = factory_value(); 
    if(l->type == e_int && r->type == e_int){
        res->boolean = is_number_equal(l->num, r->num);
    }else if(l->type == e_boolean && r->type == e_boolean){
        res->boolean = is_bool_equal(l->boolean, r->boolean);
    }else if(l->type == e_string && r->type == e_string){
        res->boolean = is_string_equal(l->str, r->str);
    }else{
        EUROPA_ERROR("You cannot compare a type of '%c' with '%c'\n", l->type, r->type);        
    }
    res->type = e_boolean;
    return res;
}

// Do the 'not equal' (!=) logic
struct e_value *do_logical_not_equal(struct e_value *l, struct e_value *r){   
    struct e_value *res = factory_value(); 
    if(l->type == e_int && r->type == e_int){
        res->boolean = is_number_not_equal(l->num, r->num);
    }else if(l->type == e_boolean && r->type == e_boolean){
        res->boolean = is_bool_not_equal(l->boolean, r->boolean);
    }else if(l->type == e_string && r->type == e_string){
        res->boolean = is_string_not_equal(l->str, r->str);
    }else{
        EUROPA_ERROR("You cannot compare a type of '%c' with '%c'\n", l->type, r->type);        
    }
    res->type = e_boolean;
    return res;
}

struct e_value *do_logical_and(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value(); 
    if(l->boolean && r->boolean){
        res->boolean = e_true;
    }else{
        res->boolean = e_false;
    }         
    res->type = e_boolean;
    return res;
}

struct e_value *do_logical_or(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value(); 
    if(l->boolean || r->boolean){
        res->boolean = e_true;
    }else{
        res->boolean = e_false;
    }         
    res->type = e_boolean;
    return res;
}

struct e_value *do_logical_greater_then(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value(); 
    if(l->type == e_int && r->type == e_int){
        if(l->num > r->num){
            res->boolean = e_true;
        }else{
            res->boolean = e_false;
        }        
    }else{
        EUROPA_ERROR("Greater than only supports integer operators\n");
    }
    res->type = e_boolean;
    return res;   
}

struct e_value *do_logical_greater_then_or_equal(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value(); 
    if(l->type == e_int && r->type == e_int){
        if(l->num >= r->num){
            res->boolean = e_true;
        }else{
            res->boolean = e_false;
        }                 
    }else{
        EUROPA_ERROR("Greater than or equal only supports integer operators");
    }
    res->type = e_boolean;
    return res;
}

struct e_value *do_logical_less_then(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();     
    if(l->type == e_int && r->type == e_int){
        if(l->num < r->num){
            res->boolean = e_true;
        }else{
            res->boolean = e_false;
        }                 
    }else{
        EUROPA_ERROR("Less than only supports integer operators");
    }
    res->type = e_boolean;
    return res;    
}

struct e_value *do_logical_less_then_or_equal(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();     
    if(l->type == e_int && r->type == e_int){        
        if(l->num <= r->num){
            res->boolean = e_true;
        }else{
            res->boolean = e_false;
        }          
    }else{
        EUROPA_ERROR("Less than or equal only supports integer operators");
    }
    res->type = e_boolean;
    return res;   
}

// Check if str1 is equal 
// to str2 
// return: bool
e_bool is_string_equal(char *str1, char *str2){
    if(do_string_compare(str1, str2) == 0){
        return e_true;
    }else{
        return e_false;
    }
}

e_bool is_bool_equal(int b1, int b2){
    if(b1 == b2){
        return e_true;
    }else{
        return e_false;
    }
}

e_bool is_number_equal(int n1, int n2){
    if(n1 == n2){        
        return e_true;
    }else{        
        return e_false;
    }
}

e_bool is_string_not_equal(char *str1, char *str2){
    if(do_string_compare(str1, str2) != 0){
        return e_true;
    }else{
        return e_false;
    }
}

e_bool is_bool_not_equal(int b1, int b2){
    if(b1 != b2){
        return e_true;
    }else{
        return e_false;
    }
}

e_bool is_number_not_equal(int n1, int n2){
    if(n1 != n2){        
        return e_true;
    }else{        
        return e_false;
    }
}

char *do_string_concat(struct e_value *str1, struct e_value *str2){
	// large enough to hold the number
	char *src_str = NULL;
	char *dst_str = NULL;
	size_t new_str_size = -1; 
	
	dst_str = convert_to_string_value(str1);
	src_str = convert_to_string_value(str2);
	new_str_size = strlen(dst_str) + strlen(src_str); 
	// realocate the destination string and doing the concatenation
	dst_str = (char *)memm_realloc(dst_str, (new_str_size + 1));
	dst_str = strncat(dst_str, src_str, new_str_size);	
	return dst_str;
}

char *convert_to_string_value(struct e_value *v){
	char number_temp[21];	
	switch(v->type){
		case e_int:			
			snprintf(number_temp, 21, "%i", v->num);
			//return strndup(number_temp, strlen(number_temp));					    
            return _STRING_DUP(number_temp);
		case e_string:			
            return _STRING_DUP(v->str);
		case e_boolean:	
            switch(v->boolean){
                case e_true: 
                    v->str = (char *)memm_alloc(sizeof(char) * 5);                    
				    strncpy(v->str, "true\0", 5);
                    break;
                case e_false:
                    v->str = (char *)memm_alloc(sizeof(char) * 6);
				    strncpy(v->str, "false\0", 6);
                    break;
                default:
                    EUROPA_ERROR("internal error: Unknown boolean value");
            }        		
			return v->str;		    
		case e_undefined:
			// return a empty string for this case
			v->str = (char *)memm_alloc(sizeof(char));
			strncpy(v->str, "\0", 1);
			return v->str;		    
		default:			
			EUROPA_ERROR("internal error: Unable to convert string");
	}
}

// use the internal c function to 
// compare the strings
// 0: strings are equal
// -1: str1 is less than str2
// +1: str1 is greater than str2
int do_string_compare(char *src, char *dst){ 
    return strcmp(src, dst);   
}

//
// 0: numbers are equal
// -1: n1 is less than n2
// -2: n1 is less than or equal n2
// 1: n1 is greater than n2
// 2: n2 is greater than or equal n2
int do_number_compare(int n1, int n2){
    if(n1 == n2){
        return 0;
    }else if(n1 <= n2){
        return -2;
    }else if(n1 < n2){
        return -1;
    }
}

void value_eval(struct e_value *v){	
    if(v != NULL){
        switch(v->type){
            case e_string:  
                if(v->size > 0){
                    EUROPA_OUTPUT("%s\n", v->str);
                }else{
                    EUROPA_OUTPUT("");
                }                
                break;
            case e_int:                      
                EUROPA_OUTPUT("%i\n", v->num);                
                break; 
            case e_boolean:                        
                switch(v->boolean){
                    case e_true: 
                        EUROPA_OUTPUT("true\n");
                        break;
                    case e_false:
                        EUROPA_OUTPUT("false\n");
                        break;
                    default:
                        EUROPA_ERROR("internal error: Unknown boolean value");
                }
                break;  
            case e_undefined:
                // A value was undefined. Nothing to do
                // This is common in functions that 
                // doesn't return any value.
                // All function calls are initialized with 'U'
                // return value;
                EUROPA_ERROR("internal error: Undefined value");
                
            default:
                return;
                EUROPA_ERROR("internal error: Unkown variable type '%c'", v->type);
        }
    }	
}


// Evaluate the return context 
// the result is pushed on the stack
void return_eval(struct ast_node *n, struct e_context *ctxt){
    struct e_value *ret_val = expr_eval(n, ctxt);
    // push the return value into context... 
    // !!!! This should be copied before the context get destroyed !!!!
    ctxt->ret_val = ret_val;
    // set the signal return, to stop all statement evaluation
    ctxt->sig_ret = 1;        
}


struct e_value *expr_eval(struct ast_node *n, struct e_context *ctxt){
    struct e_value *l;
    struct e_value *r;

    if(n->left != NULL){
        l = expr_eval(n->left, ctxt);           
    }
    if(n->right != NULL){
        r = expr_eval(n->right, ctxt);        
    }

    switch(n->type){
        // values 
        case node_value: 
            switch(n->val->type){
                case e_boolean:
                    return n->val;                
                case e_int:
                    return n->val;                
                case e_string:
                    return n->val;                                            
                default:
                    EUROPA_ERROR("internal error: Unknown value node type '%i'\n", n->val->type);  
            }                        
        case node_fcall:

        // tokens.. 
        case node_reference:   
            return function_eval(n, ctxt);         
        case node_token:
            switch(n->token->class){       
                case reference:                                         
                    return reference_eval(n, ctxt);                                    
                case andoper: // and
                    return do_logical_and(l, r);            
                break;  
                case equal: // equal         
                    return do_logical_equal(l, r);
                break; 
                case gt: // greater than
                    return do_logical_greater_then(l, r); 
                break;        
                case gte: // greater than or equal
                    return do_logical_greater_then_or_equal(l, r); 
                break;       
                case lt: // less than
                    return do_logical_less_then(l, r); 
                break;        
                case lte: // less than or equal
                    return do_logical_less_then_or_equal(l, r); 
                break;                                 
                case notequal: // not equal
                    return do_logical_not_equal(l, r);
                break;        
                case oroper: // or   
                    return do_logical_or(l, r); 
                break;        
                case plus:
                    return do_arithmetic_sum(l, r);
                break;
                case minus:
                    return do_arithmetic_subtract(l, r);
                break;
                case multiply:
                    return do_arithmetic_multiply(l, r);
                break;
                case division:        
                    return do_arithmetic_division(l, r);
                break;
                default:
                    EUROPA_ERROR("internal error: Unknow token node type '%c'\n", n->token->class);        
            }        
            break;          
    }
    EUROPA_ERROR("internal error: Unable to handle node of type '%i'\n", n->type); 
}

