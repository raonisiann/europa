#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "europa/integer.h"
#include "list.h"
#include "context.h"
#include "memm.h"
#include "europa_error.h"


void eu_int_constructor(EuObject *object, struct list *args){
    if(args && args->size == 1){
        ((EuInteger *)object)->__int_value = atoi(args->first->data);
    }
}

char *eu_int_print(EuObject *object){
    char *buf = memm_alloc(10);
    int buf_written = 0; 
    buf_written = snprintf(buf, 10, "%d", ((EuInteger *)object)->__int_value);
    if(buf_written > 0){
        return buf;
    }
    return NULL;
}

unsigned int eu_int_size(EuInteger *object){
    return ((EuObject *)object)->__size;
}

EuObject *eu_int_new(){
    EuInteger *new_int = memm_alloc(sizeof(EuInteger));
    new_int->symbols = ht_init(30);
    object_register_class_method((EuObject *)new_int, "__sum", (void *)eu_int_sum);
    new_int->__constructor = eu_int_constructor;
    new_int->__print = eu_int_print;
    new_int->__sum = eu_int_sum;
    new_int->__sub = eu_int_sub;
    new_int->__multiply = eu_int_multiply;
    new_int->__divide = eu_int_divide;
    new_int->__size = 0;
    return (EuObject *)new_int;
}

EuObject *eu_int_sum(EuObject *l, EuObject *r){
    EuObject *sum =  eu_int_new();
    ((EuInteger *)sum)->__int_value = ((EuInteger *)l)->__int_value + ((EuInteger *)r)->__int_value;
    return sum;
}

EuObject *eu_int_sub(EuObject *l, EuObject *r){
    EuObject *sub =  eu_int_new();
    ((EuInteger *)sub)->__int_value = ((EuInteger *)l)->__int_value - ((EuInteger *)r)->__int_value;
    return sub;
}

EuObject *eu_int_multiply(EuObject *l, EuObject *r){
    EuObject *mul =  eu_int_new();
    ((EuInteger *)mul)->__int_value = ((EuInteger *)l)->__int_value * ((EuInteger *)r)->__int_value;
    return mul;
}

EuObject *eu_int_divide(EuObject *l, EuObject *r){
    EuObject *div =  eu_int_new();
    if(((EuInteger *)r)->__int_value == 0){
        EUROPA_ERROR("EXCEPTION TRIGGERED: Division by zero!");
    }
    ((EuInteger *)div)->__int_value = ((EuInteger *)l)->__int_value / ((EuInteger *)r)->__int_value;
    return div;
}