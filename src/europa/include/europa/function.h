#ifndef I_FUNCTION_H
#define I_FUNCTION_H

#include "europa/object.h"

typedef struct {
    EU_OBJECT_BASE
    void *__entry_ptr;
} EuFunction;

#endif