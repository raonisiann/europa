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

extern FILE *LEX_INPUT;
extern char europa_shell_mode;

int main(int argc, char *argv[]){

// setting the default to stdin 
    char *input_file_name;
    LEX_INPUT = LEX_INPUT_DEFAULT;
    // create the global context
    GLOBAL_CTXT = context_create(100);

    europa_buffer_init();
    functions_init();       
    lex_init();

    #ifdef DEBUG 
        europa_debug_init();
        DEBUG_OUTPUT("\n\nSTART DEBUG\n=================================\n")
    #endif 
    
	if(argc > 1){
        // parse the language as non-interactive mode 
		if((LEX_INPUT = fopen(argv[1], "r")) != NULL){
            // ues a file for input 
			input_file_name = argv[1];                   
		}else{
            EUROPA_ERROR("Unable to open file '%s'\n", argv[1]);
        }           
        eu_lang(); 
	}else{  
        europa_shell_mode = 1;    
        // parse the language as shell mode  
        eu_shell();
    }  
    return 0;
}