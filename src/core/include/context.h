#ifndef I_CONTEXT_H
#define I_CONTEXT_H

#include "hashtable.h"
#include "europa_types.h"
#include "stack.h"

// Will provide context functions and branchs
// Also provides an way to exchange information 
// between different context with "ret_val"
struct e_context {
    hashtable *symbols;   
	e_stack *stack;
	struct e_value *ret_val;	
	unsigned int sig_ret;
};

struct e_context *context_create(unsigned int sym_init_size);

// hold the global symbols. Local context are created dynamically 
// on function calls
struct e_context *context_europa_global;

#define GLOBAL_CTXT context_europa_global

#endif