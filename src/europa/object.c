#include <stdlib.h>
#include <string.h>
#include "europa_error.h"
#include "europa/object.h"
#include "memm.h"
#include "context.h"

EuObject *object_create_instance(char *class_name, struct list *args){
    struct ht_entry *result = NULL;
    result = ht_get(GLOBAL_CTXT->symbols, class_name);
    if(result){
        //printf("Creating new => %s\n", result->object->class_name);
        EuObject *instance = ((EuObject *)result->object)->__new();
        instance->__constructor(instance, args);
        return instance;
    }
    EUROPA_ERROR("Undefined class '%s'", class_name);
    return NULL;
}

EuObject *object_create_empty(){
    EuObject *empty_obj = memm_alloc(sizeof(EuObject));
    empty_obj->ref_counter = 0;
    empty_obj->size = 0;
    empty_obj->class_name = "";
    empty_obj->symbols = ht_init(30);
    empty_obj->ctxt = NULL;
    empty_obj->__constructor = NULL;
    empty_obj->__new = NULL;
    empty_obj->__print = NULL;
    empty_obj->__size = 0;
    return empty_obj;
}

void object_create_class(char *class_name, EuObject *(*initializer)()){
    EuObject *new_class = object_create_empty();
    new_class->class_name = class_name;
    new_class->__new = initializer;
    ht_set_key(GLOBAL_CTXT->symbols, class_name, new_class);
}

void object_register_class_method(EuObject *class, char *key, void *method){
    ht_set_key(class->symbols, key, method);
}