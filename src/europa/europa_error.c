#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "europa_error.h"
#include "europa_io.h"
#include "lexer.h"

extern char europa_shell_mode;

void europa_raise_error(char *s, ...){
    char output_string[256];    
    va_list ap;
	va_start(ap, s);	
    vsprintf(output_string, s, ap);	
    va_end(ap);    
    EUROPA_OUTPUT("\nERROR ==>> %s\n\n", output_string);
    if(europa_shell_mode == 0){
        printf("Program is terminating...\n");
        exit(-1);
    }    
}

