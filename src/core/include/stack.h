#ifndef I_STACK_H
#define I_STACK_H

#define STACK_BASE_INDEX 0
#define STACK_OUT_INDEX -1

typedef struct europa_stack_item {
    void *data; 
} e_stack_i;

typedef struct europa_stack {
    e_stack_i *table;
    int top;     
    unsigned int free;
} e_stack;

e_stack *stack_init(unsigned int stk_size);
void *stack_pop(e_stack *stk);
void *stack_peek(e_stack *stk);
void stack_push(e_stack *stk, void *data);
void stack_destroy(e_stack *stk);

#endif 