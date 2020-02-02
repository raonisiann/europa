#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "europa_debug.h"

FILE *fp = NULL;

void europa_debug_output(char *s, ...){    
    va_list ap;
    va_start(ap, s);  
    vfprintf(fp, s, ap);
    va_end(ap);
    fflush(fp);
}

// initialize debug log file
void europa_debug_init(){	
	if(fp == NULL){
		fp = fopen("debug.txt", "a");    
	} 	
}

// Close the debug log file
void europa_debug_end(){
	if(fp != NULL){
		fclose(fp);
	}
}
