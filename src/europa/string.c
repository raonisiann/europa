#include <stdio.h>
#include "memm.h"
#include "europa/string.h"

void eu_string_constructor(EuObject *object, struct list *args){
    ((EuString *)object)->__str_value = (char *)args->first->data;
}

char *eu_string_print(EuObject *object){
    return ((EuString *)object)->__str_value;
}

EuObject *eu_string_new(){
    EuString *new_string = memm_alloc(sizeof(EuString));
    if(new_string){
        new_string->__constructor = eu_string_constructor;
        new_string->__print = eu_string_print;
        new_string->__size = 0;
        return (EuObject *)new_string;
    }
    return NULL;
}