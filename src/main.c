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
		eu_switch_file_context(argv[1]);    
        eu_lang(); 
	}else{  
        europa_shell_mode = 1;    
		eu_switch_file_context(EU_SHELL_FILE_NAME);
        // parse the language as shell mode  
        eu_shell();
    }  
    return 0;
}
