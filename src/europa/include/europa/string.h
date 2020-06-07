#ifndef I_STRING_H
#define I_STRING_H

#include "europa/object.h"

typedef struct {
    EU_OBJECT_BASE
    char *__str_value;
} EuString;

void eu_string_constructor(EuObject *object, struct list *args);
char *eu_string_print(EuObject *object);
EuObject *eu_string_new();

#endif