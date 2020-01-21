#ifndef I_DEBUG_H
#define I_DEBUG_H

#ifdef DEBUG 
#define DEBUG_OUTPUT(fmttext, ...) sd_debug_output(fmttext, ## __VA_ARGS__);
#else 
#define DEBUG_OUTPUT(fmttext, ...) 
#endif

void sd_debug_output(char *s, ...);
void sd_debug_init();
void sd_debug_end();

#endif