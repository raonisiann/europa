#include <stdio.h> // printf, FILE, stdin
#include "lexer.h"
#include "parser.h"
#include "hashtable.h"
#include "europa_debug.h"
#include "europa_error.h"

hashtable* symbols;
extern FILE *LEX_INPUT;

int main(int argc, char *argv[]){

// setting the default to stdin 
    char *input_file_name;
    LEX_INPUT = LEX_INPUT_DEFAULT;
    // 
    symbols = ht_init(100);
   
    #ifdef DEBUG 
        europa_debug_init();
        DEBUG_OUTPUT("\n\nSTART DEBUG\n=================================\n")
    #endif 
   
	if(argc > 1){
		if((LEX_INPUT = fopen(argv[1], "r")) != NULL){
            // ues a file for input 
			input_file_name = argv[1];                   
		}else{
            EUROPA_ERROR("Unable to open file '%s'\n", argv[1]);
        }
	}else{
        printf("Europa 0.1 \n");
    }  

    // do what you have to do
    return parser_start();    
}