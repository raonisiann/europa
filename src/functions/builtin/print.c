#include <stdio.h> 
#include "functions.h"

#define PRINT_LIB_NAME print

// function list published by the library
struct e_reference metadata[] = {
    BUILTIN_FUNC_METADATA_ENTRY(print, foo),  
    BUILTIN_FUNC_METADATA_ENTRY(print, bar),
    BUILTIN_FUNC_METADATA_ENTRY(print, bletch),
    BUILTIN_FUNC_METADATA_ENTRY(print, print)
};

// get the size of the metadata list 
#define print_functions_count (sizeof(metadata) / sizeof(struct e_reference))

BUILTIN_FUNC_LOAD(print){   
    for(int i = 0; i < print_functions_count; i++){        
        set_ht_reference(&metadata[i], GLOBAL_CTXT);    
    }
}

BUILTIN_FUNC_DEF(print, foo){		
	printf("Foo\n");
}

BUILTIN_FUNC_DEF(print, bar){		
	printf("Bar\n");
}

BUILTIN_FUNC_DEF(print, bletch){		
	printf("Bletch\n");
}

//
// struct i_data *internal_func_io_print(struct list *args, struct e_context *ctxt)
// 
BUILTIN_FUNC_DEF(print, print){	
    if(args->size != 1){
        return; 
    }
    // eval the first arg and display the result
    value_eval((struct e_value *)args->first->data);
}
