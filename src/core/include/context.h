#ifndef I_CONTEXT_H
#define I_CONTEXT_H

#include "hashtable.h"
#include "europa_types.h"

// Will provide context functions and branchs
// Also provides an way to exchange information 
// between different context with "ret_val"
struct e_context {
    hashtable *local_symbols;   
	struct e_value *ret_val;	
	unsigned int sig_ret;
};

#endif