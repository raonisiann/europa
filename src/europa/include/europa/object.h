#ifndef I_OBJECT_H
#define I_OBJECT_H

#include "list.h"
#include "hashtable.h"

typedef struct __eu_object EuObject;

#define EU_OBJECT_BASE \
    unsigned int ref_counter; \
    unsigned int size; \
    hashtable *symbols; \
    char *class_name; \
    struct e_context *ctxt; \
    EuObject *(*__new)(); \
    char *(*__print)(EuObject *object); \
    unsigned int __size; \
    void (*__constructor)(EuObject *object, struct list *args); \

struct __eu_object {
    EU_OBJECT_BASE
};

EuObject *object_create_instance(char *class_name, struct list *args);
EuObject *object_create_empty();
void object_create_class(char *class_name, EuObject *(*initializer)());
void object_register_class_method(EuObject *class, char *key, void *method);
void object_map_attr_to_class();
void object_call_method();
void object_get_attr();
void object_set_attr();

#endif