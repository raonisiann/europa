#include <stdlib.h>
#include <string.h>
#include "europa_types.h"
#include "memm.h"


struct e_value *factory_value(){
    return (struct e_value *)memm_alloc(sizeof(struct e_value));    
}

// create an integer value
struct e_value *create_int_value(struct lex_token *token){    
    struct e_value *v = factory_value();  
    v->type = integer; // integer type
    v->size = sizeof(int);
    v->num = atoi(token->raw_value);
    return v;
}

// create an boolean e_value from the strings "true" and "false"
struct e_value *create_boolean_value(struct lex_token *token){    
    struct e_value *v = factory_value();
    v->type = boolean; // boolean type
    v->size = sizeof(int);
    if(strcmp(token->raw_value, "true")){
        v->boolean = true;
    }else if(strcmp(token->raw_value, "false")){
        v->boolean = false;
    }else{
        //
        // raise an error.... 
        //
    }    
    return v;
}

// create an string e_value 
struct e_value *create_string_value(struct lex_token *token){   
    struct e_value *v = factory_value();
    v->type = string; // string type
    v->size = token->size;
    v->str = token->raw_value;
    return v;    
}

// TODO: Implement this method
struct e_value *create_reference_value(struct lex_token *token){
    struct e_value *v = factory_value();  
    return v;
}