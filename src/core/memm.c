#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memm.h"

void *memm_alloc(size_t size){
    void *alloc = (void *)malloc(size);
    //printf("MEMM -> ALLOCATION(%p) SIZE(%i)\n", alloc, (int)size);
    return alloc;
}

void *memm_calloc(size_t numitems, size_t size){
    void *alloc = (void *)calloc(numitems, size);
    //printf("MEMM -> ALLOCATION(%p) SIZE(%i) BLOCKS(%i)\n", alloc, (int)size, (int)numitems);
    return alloc;
}

void *memm_set(void *ptr, int value, size_t size){
    memset(ptr, value, size);
}

void *memm_realloc(void *ptr, size_t size){
    void *alloc = (void *)realloc(ptr, size);
    //printf("MEMM -> REALLOCATION(%p) NEWSIZE(%i)\n", alloc, (int)size);
    return alloc;
}

void memm_free(void *ptr){
    //printf("MEMM -> FREE(%p)\n", ptr);    
    free(ptr);
}