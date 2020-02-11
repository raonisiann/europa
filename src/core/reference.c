#include "reference.h"
#include "expr.h"
#include "hashtable.h"
#include "memm.h"
#include "europa_error.h"
#include "europa_debug.h"
#include "statement.h"
#include "context.h"
#include "stack.h"

struct e_value *reference_eval(struct ast_node *ref_node, struct e_context *ctxt){	
	struct e_reference *result = NULL;
	result = get_ht_reference(ref_node->token->raw_value, ctxt);
	if(result){
		DEBUG_OUTPUT("REFERENCE_FOUND");
		return result->value;
	}else{
		EUROPA_ERROR("Reference '%s' not found", ref_node->token->raw_value);
		return NULL;
	}
}

void function_param_map(struct e_reference *fdef, struct ast_node *fcall_node){
	struct list_item *input_arg = NULL, *def_arg = NULL;	
	struct e_value *input_value = NULL;
	struct e_reference *new_param_ref = NULL;
	
	if(fdef->funcdef->arg_list->size != fcall_node->fcall->expr_list->size){
		EUROPA_ERROR("Wrong number of parameters for function '%s', is expecting %i, but you provided %i\n", fcall_node->token->raw_value, fdef->funcdef->arg_list->size, fcall_node->fcall->expr_list->size);
	}

	input_arg = fcall_node->fcall->expr_list->first;
	def_arg = fdef->funcdef->arg_list->first;
	while(input_arg != NULL){		
		DEBUG_OUTPUT("INPUT_ARG_EXPR_EVAL\n");
		assignment_eval(((struct ast_node *)def_arg->data)->token, (struct ast_node *)input_arg->data, fdef->funcdef->ctxt);
		DEBUG_OUTPUT("NEXT_ITEM\n");
		input_arg = input_arg->next;
		def_arg = def_arg->next;
	}
}

struct e_value *function_eval(struct ast_node *fcall_node){
	struct e_reference *fdef = NULL;
	struct e_value *ret_val = NULL; 
	// function eval are placed on global table by default
	fdef = get_ht_reference(fcall_node->fcall->func->raw_value, GLOBAL_CTXT);	
	if(fdef){
		DEBUG_OUTPUT("REFERENCE_FOUND\n");		
		function_param_map(fdef, fcall_node);	
		stmt_block_eval(fdef->funcdef->body, fdef->funcdef->ctxt);		
		ret_val = (struct e_value *)stack_pop(fdef->funcdef->ctxt->stack);
		return ret_val;
	}else{
		EUROPA_ERROR("Reference for function '%s' not found\n", fcall_node->fcall->func->raw_value);
		return ret_val;
	}
}

void assignment_eval(struct lex_token *ref_tk, struct e_value *v, struct e_context *ctxt){
	// 
	DEBUG_OUTPUT("ASSIGNMENT_EVAL -> '%s'", ref_tk->raw_value);	
	struct e_reference *found_ref = NULL; 
	found_ref = get_ht_reference(ref_tk->raw_value, ctxt);
	// new reference...
	if(found_ref){	
	// otherwise just a re-assign... 
		DEBUG_OUTPUT("REASSIGN");
		found_ref->value = v;
	}else{
		DEBUG_OUTPUT("NEW_REFERENCE");
		struct e_reference *new_ref = factory_reference();
		new_ref->name = ref_tk->raw_value;
		new_ref->type = e_reference;	
		new_ref->value = v;		
		set_ht_reference(new_ref, ctxt);
	}	
	DEBUG_OUTPUT("DONE_ASSIGMENT");
}

// create an entry on the hashtable for reference 
//
// A reference is always created on the local context if exists 
// otherwise, it is placed on the global 
void set_ht_reference(struct e_reference *ref, struct e_context *ctxt){
	ref->ht_hash = ht_key_calc(ref->name);
	if(ctxt != NULL){
		DEBUG_OUTPUT("NEW_LOCAL_REFERENCE");
		ht_set(ctxt->symbols, ref);
	}else{
		DEBUG_OUTPUT("NEW_GLOBAL_REFERENCE");
		ht_set(GLOBAL_CTXT->symbols, ref);
	}			
}


// return an reference from the hashtable
//
// Resolution order: 
// 1. 	Look at the local context at 'ctxt' if defined
// 2.	If found on local, return the value
// 3. 	If not found on local, look at the global context
// 4.	If not found on global, neither local, return NULL	
struct e_reference *get_ht_reference(char *name, struct e_context *ctxt){	
	struct ht_entry *find_ref = NULL;
	if(ctxt != NULL){
		DEBUG_OUTPUT("LOOKING_LOCAL_CONTEXT");
		find_ref = ht_get(ctxt->symbols, name);		
		if(find_ref != NULL){
			DEBUG_OUTPUT("FOUND_AT_LOCAL_CONTEXT");
			return find_ref->data;
		}		
	}
	DEBUG_OUTPUT("LOOKING_GLOBAL_LOCAL_CONTEXT");
	// look at global 
	find_ref = ht_get(GLOBAL_CTXT->symbols, name);	
	if(find_ref != NULL){
		DEBUG_OUTPUT("FOUND_AT_GLOBAL_LOCAL_CONTEXT");
		return find_ref->data;
	}else{
		DEBUG_OUTPUT("REFERENCE_NOT_FOUND");
		return NULL;
	}
}

// assign a value to reference 
void set_value_to_reference(struct e_reference *ref, struct e_value *v){	
	ref->value = v; 
}
