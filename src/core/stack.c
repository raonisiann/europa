#include <stdio.h>
#include "stack.h"
#include "europa_error.h"
#include "memm.h"

// Create and stack of the size 'stk_size' 
// Returns the pointer to the new created stack 'e_stack'
e_stack *stack_init(unsigned int stk_size){	 
    e_stack *stack_new = (e_stack *)memm_alloc(sizeof(e_stack));
    if(stack_new == NULL){
        // this should be an FATAL error instead...
        // out of resources it's something that we can't ignore 
        EUROPA_ERROR("Unable to allocate memory for stack");
    }	
    // allocate memory for all stack elements 
    stack_new->table = (e_stack_i *)memm_calloc(stk_size, sizeof(e_stack_i));
    if(stack_new->table == NULL){
        EUROPA_ERROR("Unable to allocate memory for stack elements");
    }
    stack_new->free = stk_size; 
    stack_new->top = STACK_OUT_INDEX; // out of the stack bounds... means empty stack
    return stack_new;
}


// Returns an item from the stack
// decrement the stk->top and increment 
// the stk->free counters... 
e_stack_i *stack_pop(e_stack *stk){

    if(stk->top == STACK_OUT_INDEX){
        return NULL;
    }
    e_stack_i *item = &stk->table[stk->top];
    stk->top = stk->top - 1;
    stk->free = stk->free + 1;
    return item; 
}


// Return an item from top without remove it
e_stack_i *stack_peek(e_stack *stk){

    if(stk->top == STACK_OUT_INDEX){
        return NULL;
    }
    return &stk->table[stk->top];
}


// Push an item to stack top 
// Pushing an item to a full stack will cause 
// an error and program will terminate. 
// So, measure your stack before start pushing items
void stack_push(e_stack *stk, e_stack_i *item){

    if(stk->free == 0){
        EUROPA_ERROR("Stack full");        
    }
    unsigned int new_top = stk->top + 1;
    stk->table[new_top];
    stk->free = stk->free -1;
    stk->top = new_top;
}


// Destroy the stack and it items.
// This cannot be undone. 
void stack_destroy(e_stack *stk){

    memm_free(stk->table);
    memm_free(stk);
}