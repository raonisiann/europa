#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memm.h"
#include "expr.h"
#include "europa_types.h"
#include "lexer.h"
#include "reference.h"


/*
struct e_value *set_variable_value(char sym[32]){
    struct e_value *d = (struct e_value *)memm_alloc(sizeof(struct e_value)); 
    d->type = 'V'; // string type
    d->ht_hash = ht_key_calc(sym);
    d->str = strdup(sym);   
    d->func = NULL;
    return d; 
}
*/

/*
struct e_value *set_fcall_value(struct func_def *f){
    struct e_value *d = (struct e_value *)memm_alloc(sizeof(struct e_value)); 
    d->type = 'C'; // function call value
    d->ht_hash = ht_key_calc(f->name);    
    d->func = f; 
    return d; 
}
*/

// Set the token type
// like '*', '/', etc... used to build
// the ASTs
struct e_value *set_token_type(char value){
    struct e_value *d = (struct e_value *)memm_alloc(sizeof(struct e_value));
    d->type = value;    
    return d;    
}

/*
bool get_bool_value(struct ast_node *n){
    return n->data->boolean;
}
*/

/*
char *get_string_value(struct ast_node *n){
    return n->data->str;
}
*/

struct e_value *do_arithmetic_sum(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();
	if(l->type == string || r->type == string){
		// this is considered an string concatenation
		res->str = do_string_concat(l, r);
		res->type = string;
	}else{
		res->num = l->num + r->num;
		res->type = integer;
	}    
    return res;
}

struct e_value *do_arithmetic_subtract(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();
    res->num = l->num - r->num;
    res->type = integer;
    return res;
}

struct e_value *do_arithmetic_multiply(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();
    res->num = l->num * r->num;
    res->type = integer;
    return res;
}

struct e_value *do_arithmetic_division(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();
    res->num = (l->num / r->num);
    res->type = integer;
    return res;
}

// Do the 'equal' logic
struct e_value *do_logical_equal(struct e_value *l, struct e_value *r){   
    struct e_value *res = factory_value(); 
    if(l->type == integer && r->type == integer){
        res->boolean = is_number_equal(l->num, r->num);
    }else if(l->type == boolean && r->type == boolean){
        res->boolean = is_bool_equal(l->boolean, r->boolean);
    }else if(l->type == string && r->type == string){
        res->boolean = is_string_equal(l->str, r->str);
    }else{
        printf("You cannot compare a type of '%c' with '%c'\n", l->type, r->type);
        exit(-1);
    }
    res->type = 'b';
    return res;
}


// Do the 'not equal' (!=) logic
struct e_value *do_logical_not_equal(struct e_value *l, struct e_value *r){   
    struct e_value *res = factory_value(); 
    if(l->type == integer && r->type == integer){
        res->boolean = is_number_not_equal(l->num, r->num);
    }else if(l->type == boolean && r->type == boolean){
        res->boolean = is_bool_not_equal(l->boolean, r->boolean);
    }else if(l->type == string && r->type == string){
        res->boolean = is_string_not_equal(l->str, r->str);
    }else{
        printf("You cannot compare a type of '%c' with '%c'\n", l->type, r->type);
        exit(-1);
    }
    res->type = boolean;
    return res;
}

struct e_value *do_logical_and(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value(); 
    res->boolean = l->boolean && r->boolean;
    res->type = boolean;
    return res;
}

struct e_value *do_logical_or(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value(); 
    res->boolean = l->boolean || r->boolean;
    res->type = boolean;
    return res;
}

struct e_value *do_logical_greater_then(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value(); 
    if(l->type == integer && r->type == integer){
        res->boolean = l->num > r->num;
    }else{
        printf("Greater than only supports integer operators\n");
    }
    res->type = boolean;
    return res;   
}

struct e_value *do_logical_greater_then_or_equal(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value(); 
    if(l->type == integer && r->type == integer){
        res->boolean = l->num >= r->num;
    }else{
        printf("Greater than or equal only supports integer operators\n");
    }
    res->type = boolean;
    return res;
}

struct e_value *do_logical_less_then(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();     
    if(l->type == integer && r->type == integer){
        res->boolean = l->num < r->num;
    }else{
        printf("Less than only supports integer operators\n");
    }
    res->type = boolean;
    return res;    
}

struct e_value *do_logical_less_then_or_equal(struct e_value *l, struct e_value *r){
    struct e_value *res = factory_value();     
    if(l->type == integer && r->type == integer){
        res->boolean = l->num <= r->num;
    }else{
        printf("Less than or equal only supports integer operators\n");
    }
    res->type = boolean;
    return res;   
}

// Check if str1 is equal 
// to str2 
// return: bool
bool is_string_equal(char *str1, char *str2){
    int result;
    result = do_string_compare(str1, str2);
    if(result == 0){
        return true;
    }else{
        return false;
    }
}

bool is_bool_equal(int b1, int b2){
    if(b1 == b2){
        return true;
    }else{
        return false;
    }
}

bool is_number_equal(int n1, int n2){
    if(n1 == n2){        
        return true;
    }else{        
        return false;
    }
}

bool is_string_not_equal(char *str1, char *str2){
    int result;
    result = do_string_compare(str1, str2);
    if(result != 0){
        return true;
    }else{
        return false;
    }
}

bool is_bool_not_equal(int b1, int b2){
    if(b1 != b2){
        return true;
    }else{
        return false;
    }
}

bool is_number_not_equal(int n1, int n2){
    if(n1 != n2){        
        return true;
    }else{        
        return false;
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
	dst_str[new_str_size + 1] = '\0';
	return dst_str;
}

char *convert_to_string_value(struct e_value *v){
	char number_temp[21];	
	switch(v->type){
		case integer:			
			snprintf(number_temp, 21, "%i", v->num);
			return strndup(number_temp, strlen(number_temp));					    
		case string:
			return strndup(v->str, strlen(v->str));		
		case boolean:			
			if(v->boolean == 0){
				v->str = (char *)memm_alloc(sizeof(char) * 6);
				strncpy(v->str, "false\0", 6);
			}else{
				v->str = (char *)memm_alloc(sizeof(char) * 5);
				strncpy(v->str, "true\0", 5);
			}
			return v->str;
		break;
		case 'U':
			// return a empty string for this case
			v->str = (char *)memm_alloc(sizeof(char));
			strncpy(v->str, "\0", 1);
			return v->str;
		break;
		default:			
			printf("Some crazy error\n");
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
	//DEBUG_OUTPUT("Evaluating EXPR of type -> %c\n", data->type);
    switch(v->type){
        case string:               
            //SD_LBUF_OUTPUT("%s\n", data->str);           
            printf("%s\n", v->str);
            break;
        case integer:          
            //SD_LBUF_OUTPUT("%li\n", data->num);
            printf("%i\n", v->num);
            break; 
        case boolean:            
            if(v->boolean == true){
                printf("true\n");
            }else{
                printf("false\n");
            }            
            break;  
		case undefined:
			// A value was undefined. Nothing to do
			// This is common in functions that 
			// doesn't return any value.
			// All function calls are initialized with 'U'
			// return value;
			return;		    
        default:
			return;
            printf("internal error: Unkown variable type '%c'", v->type);
    }	
}



struct e_value *expr_eval(struct ast_node *n){
    struct e_value *l;
    struct e_value *r;

    if(n->left != NULL){
        l = expr_eval(n->left);           
    }
    if(n->right != NULL){
        r = expr_eval(n->right);        
    }

    switch(n->token->class){    
        case boolean:
			return create_boolean_value(n->token);
		break;
        case integer:
			return create_int_value(n->token);
		break;
        case string:
			return create_string_value(n->token);            
        break;           
        case 'A': // and
            return do_logical_and(l, r);            
        break;  
		case 'C':
            //return function_eval(n->data);			
		break;
        case 'E': // equal         
            return do_logical_equal(l, r);
        break; 
        case 'g': // greater than
            return do_logical_greater_then(l, r); 
        break;        
        case 'G': // greater than or equal
            return do_logical_greater_then_or_equal(l, r); 
        break;       
        case 'l': // less than
            return do_logical_less_then(l, r); 
        break;        
        case 'L': // less than or equal
            return do_logical_less_then_or_equal(l, r); 
        break;                                 
        case 'N': // not equal
            return do_logical_not_equal(l, r);
        break;        
        case 'O': // or   
            return do_logical_or(l, r); 
        break;        
        case reference:                                                  
            // return var_eval_with_hash(n->token);            
            // TODO: eval_reference
            return reference_eval(n);
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
            printf("Unknow node type '%c'\n", n->token->class);        
    }
}

