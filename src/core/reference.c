#include "reference.h"
#include "expr.h"
#include "hashtable.h"
#include "memm.h"
#include "europa_error.h"
#include "europa_debug.h"

extern hashtable* symbols;

struct e_reference *reference_factory(){
	return (struct e_reference *)memm_alloc(sizeof(struct e_reference));
}

struct e_value *reference_eval(struct ast_node *n){	
	struct ht_entry *result = NULL;
	result = ht_get(symbols, n->token->raw_value);
	if(result){
		DEBUG_OUTPUT("REFERENCE_FOUND");
		return result->data->value;
	}else{
		EUROPA_ERROR("Reference '%s' not found", n->token->raw_value);
		return NULL;
	}
}

void assignment_eval(struct ast_assignment_node *node){
	// 
	DEBUG_OUTPUT("NEW_REFERENCE '%s'", node->reference->token->raw_value);
	struct e_value *expr_value = expr_eval(node->assigment);	
	create_reference(node->reference->token->raw_value, expr_value);
}


void create_reference(char *name, struct e_value *value){    
	struct e_reference *ref = reference_factory();
	ref->name = name; 
	ref->value = value;
	ref->ht_hash = ht_key_calc(name);

	ht_set(symbols, ref);
	/*
    if(d->ht_hash != 0){        
        e = ht_get_by_hash(symbols, (char *)sym, d->ht_hash);
    }else{
        e = ht_get(symbols, (char *)sym);
    }    
	// the symbol is defined at the GLOBAL symbols	
	if(e != NULL){		
        ht_set(symbols, sym, d);		
	}else{
		// the symbol is not defined at the GLOBAL table
		// and we have LOCAL context
		if(c != NULL){			
			ht_set(c->local_symbols, sym, d);			
		}else{			
			ht_set(symbols, sym, d);
		}
	}
	*/
}
