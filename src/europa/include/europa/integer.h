#ifndef I_INTEGER_H
#define I_INTEGER_H

#include "europa/object.h"

typedef struct {
    EU_OBJECT_BASE
    int __int_value;
    EuObject *(*__sum)(EuObject *, EuObject *);
    EuObject *(*__sub)(EuObject *, EuObject *);
    EuObject *(*__multiply)(EuObject *, EuObject *);
    EuObject *(*__divide)(EuObject *, EuObject *);
} EuInteger;

EuObject *eu_int_new();
EuObject *eu_int_sum(EuObject *l, EuObject *r);
EuObject *eu_int_sub(EuObject *l, EuObject *r);
EuObject *eu_int_multiply(EuObject *l, EuObject *r);
EuObject *eu_int_divide(EuObject *l, EuObject *r);
void eu_int_constructor(EuObject *object, struct list *args);
char *eu_int_print(EuObject *object);

#endif