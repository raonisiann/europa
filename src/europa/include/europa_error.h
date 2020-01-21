#ifndef I_EUROPA_ERROR_H
#define I_EUROPA_ERROR_H


#define EUROPA_ERROR(TEXT, ... ) europa_raise_error(TEXT, ## __VA_ARGS__)

void europa_raise_error(char *s, ...);

#endif