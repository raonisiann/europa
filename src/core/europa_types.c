#include <stdlib.h>
#include <string.h>
#include "europa_types.h"
#include "europa_io.h"
#include "europa_error.h"
#include "memm.h"


struct e_value *factory_value(){
    struct e_value *new_value = (struct e_value *)memm_alloc(sizeof(struct e_value));
    new_value->type = e_undefined;
    new_value->size = 0;
    new_value->str = NULL;
    return new_value;    
}

struct e_value *token_to_integer(struct lex_token *token){
    struct e_value *v = factory_value();  
    v->type = e_int; 
    v->size = sizeof(int);
    v->num = atoi(token->raw_value);
    return v;    
}

struct e_value *token_to_string(struct lex_token *token){
    struct e_value *v = factory_value();  
    v->type = e_string; 
    v->size = token->size;
    v->str = token->raw_value;
    return v;   
}

struct e_value *token_to_boolean(struct lex_token *token){
    struct e_value *v = factory_value();
    v->type = e_boolean; // boolean type
    v->size = sizeof(int);    
    if(strcmp(token->raw_value, "true") == 0){
        v->boolean = e_true;        
    }else if(strcmp(token->raw_value, "false") == 0){        
        v->boolean = e_false;
    }else{
        EUROPA_ERROR("Invalid boolean value");
    }        
    return v;
}

struct e_reference *factory_reference(){
    struct e_reference *new_ref = (struct e_reference *)memm_alloc(sizeof(struct e_reference));
    new_ref->type = e_undefined;
    new_ref->name = NULL;
    new_ref->ht_hash = 0;  
    new_ref->value = NULL;  
    return new_ref;
}

struct e_reference *new_reference(char *ref_name, struct e_value *val){
    struct e_reference *ref = factory_reference();
    ref->type = e_reference;
    ref->name = ref_name;
    ref->ht_hash = 0; 
    ref->value = val;
}

struct e_reference *new_fcall(char *func_name, struct list *args){
    struct e_reference *func = factory_reference();
    func->fcall = factory_fcall();
    func->fcall->args = args;
    func->type = e_fcall;
    func->name = func_name;
    func->ht_hash = 0; 
    return func;
}

struct e_fcall *factory_fcall(){
    struct e_fcall *new_fcall = (struct e_fcall *)memm_alloc(sizeof(struct e_fcall));
    new_fcall->args = NULL;
    new_fcall->ret = NULL;
    return new_fcall;
}

struct e_assignment *factory_assigment(){
	struct e_assignment *new_assignment = (struct e_assignment *)memm_alloc(sizeof(struct e_assignment));
	new_assignment->ref = NULL;
	new_assignment->ast = NULL;
	return new_assignment;
}

struct e_assignment *create_assignment(struct e_reference *ref, struct ast_node *n){
	struct e_assignment *assignment = factory_assigment();
	assignment->ref = ref; 
	assignment->ast = n;
    return assignment;
}
