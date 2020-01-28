#include "reference.h"
#include "expr.h"
#include "hashtable.h"
#include "memm.h"
#include "europa_error.h"
#include "europa_debug.h"

extern hashtable* symbols;

struct e_value *reference_eval(struct e_reference *ref){	
	struct ht_entry *result = NULL;
	result = ht_get(symbols, ref->name);
	if(result){
		DEBUG_OUTPUT("REFERENCE_FOUND");
		return result->data->value;
	}else{
		EUROPA_ERROR("Reference '%s' not found", ref->name);
		return NULL;
	}
}

void assignment_eval(struct e_assignment *assignment){
	// 
	DEBUG_OUTPUT("NEW_REFERENCE '%s'", assignment->ref->name);
	create_reference(assignment->ref->name, expr_eval(assignment->ast));
}


void create_reference(char *name, struct e_value *value){    
	struct e_reference *ref = factory_reference();
	ref->name = name; 
	ref->type = e_reference;
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
