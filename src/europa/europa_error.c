#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "europa_error.h"
#include "europa_io.h"
#include "lexer.h"


void europa_raise_error(char *s, ...){
    char output_string[100];    
    va_list ap;
	va_start(ap, s);	
    vsprintf(output_string, s, ap);	
    va_end(ap);    
    EUROPA_OUTPUT("\nERROR ==>> %s\n\nProgram is terminating...\n", output_string);
    exit(-1);
}

