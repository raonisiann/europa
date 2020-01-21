// controls the output buffer
#include <stdio.h>
#include <stdarg.h>
#include "europa_io.h"

void europa_buffer_append(FILE *buffer, char *s, ...){
    va_list ap;
	va_start(ap, s);	
	vfprintf(buffer, s, ap);	
    va_end(ap);
}
