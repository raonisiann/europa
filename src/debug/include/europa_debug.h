#ifndef I_DEBUG_H
#define I_DEBUG_H

#ifdef DEBUG 
#define DEBUG_OUTPUT(fmttext, ...) europa_debug_output(fmttext, ## __VA_ARGS__);
#else 
#define DEBUG_OUTPUT(fmttext, ...) 
#endif

void europa_debug_output(char *s, ...);
void europa_debug_init();
void europa_debug_end();

#endif