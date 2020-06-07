#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// holds the current library list to be loaded
struct function_lib_metadata functions_lib_list[] = {
    BUILTIN_LIB_ENTRY(print)
};

// get the size of the function list 
#define functions_lib_count (sizeof(functions_lib_list) / sizeof(struct function_lib_metadata))

// Called by the main method to load all builtin functions
void functions_init(){
    for(int i = 0; i < functions_lib_count; i++){
        DEBUG_OUTPUT("Loading library '%s'...", functions_lib_list[i].lib);
        functions_lib_list[i].loader();
    }
}