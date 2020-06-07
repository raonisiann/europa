#ifndef I_BOOLEAN_H
#define I_BOOLEAN_H

#include "europa/object.h"

typedef struct {
    EU_OBJECT_BASE
    unsigned int __bool_value;
    unsigned int (*__is_true)(EuObject *);
    unsigned int (*__is_false)(EuObject *);
} EuBoolean;

unsigned int eu_boolean_is_true(EuObject *object);
unsigned int eu_boolean_is_false(EuObject *object);
char *eu_boolean_print(EuObject *object);
void eu_boolean_constructor(EuObject *object, struct list *args);
EuObject *eu_boolean_new();

#endif