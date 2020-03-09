#include <stdio.h> // printf, FILE, stdin
#include "europa.h"
#include "lexer.h"
#include "parser.h"
#include "hashtable.h"
#include "europa_debug.h"
#include "europa_error.h"
#include "europa_io.h"
#include "context.h"
#include "functions.h"

extern char europa_shell_mode;

int main(int argc, char *argv[]){
    #ifdef DEBUG 
        europa_debug_init();
        DEBUG_OUTPUT("\n\nSTART DEBUG\n=================================\n")
    #endif 
    	
	eu_init();
    // create the global context
    GLOBAL_CTXT = context_create(100);
    
    functions_init();           

	if(argc > 1){
		push_to_include_stack(factory_file_desc(argv[1]));    
		lex_init();		
        eu_lang(); 
	}else{  
        europa_shell_mode = 1;    
		push_to_include_stack(factory_file_desc(EU_SHELL_FILE_NAME));
		lex_init();		
        // parse the language as shell mode  
        eu_shell();
    }  
    return 0;
}