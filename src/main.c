#include <stdio.h> // printf, FILE, stdin
#include "europa.h"
#include "lexer.h"
#include "parser.h"
#include "hashtable.h"
#include "europa_debug.h"
#include "europa_error.h"
#include "europa_io.h"
#include "context.h"
#include "functions.h"
#include "europa/integer.h"
#include "europa/boolean.h"
#include "europa/string.h"

extern char europa_shell_mode;

int main(int argc, char *argv[]){
    // Whats next
    printf("Whats next?\n");
    printf("\n");
    printf("    - Make arithmetical and logical operations work with objects\n");
    printf("    - Make function definition works with objects\n");
    printf("    - Force the use of ht_set_key instead of ht_set\n");
    printf("    - Implement the 'null' object\n");
    printf("    - Improve builtin types (string, int, boolean, null)\n");
    printf("\n\n");

    #ifdef DEBUG
        europa_debug_init();
        DEBUG_OUTPUT("\n\nSTART DEBUG\n=================================\n")
    #endif
    eu_init();
    // create the global context
    GLOBAL_CTXT = context_create(100);

    functions_init();
    // Create builtin classes
    object_create_class("Integer", eu_int_new);
    object_create_class("String", eu_string_new);
    object_create_class("Boolean", eu_boolean_new);

    if(argc > 1){
        eu_switch_file_context(argv[1]);
        eu_lang();
    }else{
        europa_shell_mode = 1;
        eu_switch_file_context(EU_SHELL_FILE_NAME);
        // parse the language as shell mode
        eu_shell();
    }
    return 0;
}
