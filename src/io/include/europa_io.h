#ifndef I_EUROPA_IO_H
#define I_EUROPA_IO_H

#include <stdio.h>

#define EUROPA_OUTPUT(TEXT, ... ) europa_buffer_append(stdout, TEXT, ## __VA_ARGS__)
#define EUROPA_INPUT(TEXT, ... ) europa_buffer_append(stdin, TEXT, ## __VA_ARGS__)

void europa_buffer_append(FILE *buffer, char *s, ...);

#endif