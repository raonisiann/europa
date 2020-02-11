#include <stdio.h>
#include "context.h"
#include "hashtable.h"
#include "memm.h"

struct e_context *context_create(unsigned int sym_init_size){
    struct e_context *new_ctxt = (struct e_context *)memm_alloc(sizeof(struct e_context));
    // Create and allocate a hashtable to hold the context symbols (references and function def).
    // Hashtables are dynamically sized and this is a initial value only.    
    new_ctxt->symbols = ht_init(sym_init_size);
    new_ctxt->ret_val = NULL;
    new_ctxt->sig_ret = 0;
    return new_ctxt;
}

// Destroy an created context
void context_destroy(struct e_context *ctxt){
    // destroy the hashtable with the references
    // the values are not destroyed 
    ht_destroy(ctxt->symbols);
    // remove the references and signals
    ctxt->ret_val = NULL; 
    ctxt->sig_ret = 0;
    memm_free(ctxt);
}