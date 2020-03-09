#ifndef I_EUROPA_H
#define I_EUROPA_H
#include <stdio.h>
#include "stack.h"

#define EU_VERSION "0.1"
#define EU_BUILT_DATE __TIMESTAMP__

#if DEBUG
#define EU_DEBUG_INFO "with debug flag"
#else
#define EU_DEBUG_INFO ""
#endif

// Detect where the program is being compiled... 
// 
#ifdef _WIN32
    // Windows (x86)
    #define EU_BUILT_OS "Windows"
#elif _WIN64
    // Windows (x64)
    #define EU_BUILT_OS "Windows x64"
#elif __unix__ 
    // Unix
    #define EU_BUILT_OS "Windows"
#elif __linux__
    // linux
    #define EU_BUILT_OS "Linux"
#elif __APPLE__
    // Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
    #define EU_BUILT_OS "Mac"
#else 
    #define EU_BUILT_OS "Unkown"
#endif

// max depth allowed for include files
#define EU_INCLUDE_MAX_DEPTH 15

#define EU_SHELL_FILE_NAME "***EUROPA_SHELL***"


struct eu_file_desc {
	unsigned int line_num; 
	unsigned int char_pos;
	char *file_name; 
	FILE *fd; 
};

struct e_stack *eu_include_stack_init();
void push_to_include_stack(struct eu_file_desc *file);
struct eu_file_desc *pop_from_include_stack();
int get_include_stack_size();
struct eu_file_desc *get_current_file_desc();
struct eu_file_desc *factory_file_desc(char *fname);

void eu_init();


#define CURRENT_FILE_STRUCT get_current_file_desc()
#define CURRENT_FILE_NAME CURRENT_FILE_STRUCT->file_name
#define CURRENT_LINE_NUM CURRENT_FILE_STRUCT->line_num
#define CURRENT_CHAR_POS CURRENT_FILE_STRUCT->char_pos
#define CURRENT_FD CURRENT_FILE_STRUCT->fd

#endif