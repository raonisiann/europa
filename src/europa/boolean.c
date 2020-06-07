#include <stdio.h>
#include <string.h>
#include "memm.h"
#include "europa/boolean.h"

unsigned int eu_boolean_is_true(EuObject *object){
    if(((EuBoolean *)object)->__bool_value == 1){
        return 1;
    }
    return 0;
}
unsigned int eu_boolean_is_false(EuObject *object){
    if(((EuBoolean *)object)->__bool_value == 0){
        return 1;
    }
    return 0;
}

char *eu_boolean_print(EuObject *object){
    if(((EuBoolean *)object)->__bool_value == 1){
        return "true";
    }
    return "false";
}

void eu_boolean_constructor(EuObject *object, struct list *args){
    if(strcmp((char *)args->first->data, "true") == 0){
        ((EuBoolean *)object)->__bool_value = 1;
    }else if(strcmp((char *)args->first->data, "false") == 0){
        ((EuBoolean *)object)->__bool_value = 0;
    }
}

EuObject *eu_boolean_new(){
    EuBoolean *new_boolean = memm_alloc(sizeof(EuBoolean));
    if(new_boolean){
        new_boolean->__constructor = eu_boolean_constructor;
        new_boolean->__print = eu_boolean_print;
        new_boolean->__is_true = eu_boolean_is_true;
        new_boolean->__is_false = eu_boolean_is_false;
        new_boolean->__size = 0;
        return (EuObject *)new_boolean;
    }
    return NULL;
}