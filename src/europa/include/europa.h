#ifndef I_EUROPA_H
#define I_EUROPA_H
#include <stdio.h>
#include "stack.h"
#include "lexer.h"

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
    #include "win_api.h"
#elif _WIN64
    // Windows (x64)
    #define EU_BUILT_OS "Windows x64"
    #include "win_api.h"
#elif __unix__ 
    // Unix
    #define EU_BUILT_OS "Windows"
    #include "linux_api.h"
#elif __linux__
    // linux
    #define EU_BUILT_OS "Linux"
    #include "linux_api.h"
#elif __APPLE__
    // Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
    #define EU_BUILT_OS "Mac"
    #include "linux_api.h"
#else 
    #define EU_BUILT_OS "Unkown"
    #include "linux_api.h"
#endif

// max depth allowed for include files
#define EU_INCLUDE_MAX_DEPTH 15
#define EU_SHELL_FILE_NAME "***EUROPA_SHELL***"

struct eu_file_desc {
    char *file_name;
    struct lex_context *lex_ctxt;
    FILE *fd;
};

struct e_stack *eu_include_stack_init();
void push_to_include_stack(struct eu_file_desc *file);
struct eu_file_desc *pop_from_include_stack();
int get_include_stack_size();
struct eu_file_desc *get_current_file_desc();
char *get_eu_working_dir();
char *get_eu_root_dir();
struct eu_file_desc *factory_file_desc(char *fname);
void eu_switch_file_context(char *to_file);
void eu_init();


#define CURRENT_FILE_STRUCT get_current_file_desc()
#define CURRENT_FILE_NAME CURRENT_FILE_STRUCT->file_name
#define CURRENT_FD CURRENT_FILE_STRUCT->fd

#endif
