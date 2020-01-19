#ifndef I_MEMM_H
#define I_MEMM_H

#include <stdio.h>

void *memm_alloc(size_t size);
void *memm_calloc(size_t numitems, size_t size);
void *memm_realloc(void *ptr, size_t size);
void *memm_set(void *ptr, int value, size_t size);
void memm_free(void *ptr);

#endif