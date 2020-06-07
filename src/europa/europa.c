#include <string.h>
#include <stdlib.h>
#include "europa.h"
#include "stack.h"
#include "memm.h"
#include "europa_error.h"

char europa_shell_mode = 0;
e_stack *eu_include_stack = NULL;
struct eu_file_desc *eu_current_include_file = NULL;

// Working dir will be used as base path
// for include files as part of the include
// resolution process.
// This is mandatory and automatically defined
// during the lang startup process.
char *eu_working_dir = NULL;

// Root dir for include search.
// Lowest priority for file include resolution.
// It's defined using env EUROPA_ROOT.
// The path must be readable if defined.
// It's also not mandatory to have this define, but
// will affect the file include.
char *eu_root_dir = NULL;

void eu_init(){
    eu_working_dir = EU_GET_CWD(NULL, 0);
    if(eu_working_dir == NULL){
        EUROPA_ERROR("Unable to detect current directory. ");
        exit(-1);
    }
    eu_include_stack = stack_init(EU_INCLUDE_MAX_DEPTH);
}

void eu_switch_file_context(char *to_file){
    // create new lex_context
    struct eu_file_desc *file_desc = factory_file_desc(to_file);
    // switch lex context
    lex_switch_context(file_desc->lex_ctxt);
    // push file desc to stack and update current file pointer
    stack_push(eu_include_stack, (void *)file_desc);
    eu_current_include_file = file_desc;
}

void push_to_include_stack(struct eu_file_desc *file){
    stack_push(eu_include_stack, (void *)file);
}

struct eu_file_desc *pop_from_include_stack(){
    struct eu_file_desc *ret = NULL;
    ret = (struct eu_file_desc *)stack_pop(eu_include_stack);
    // update the current include file with
    // the top of the stack
    eu_current_include_file = (struct eu_file_desc *)stack_peek(eu_include_stack);
    lex_switch_context(ret->lex_ctxt);
    return ret;
}

int get_include_stack_size(){
    return EU_INCLUDE_MAX_DEPTH - eu_include_stack->free;
}

struct eu_file_desc *get_current_file_desc(){
    return eu_current_include_file;
}

char *get_eu_working_dir(){
    return eu_working_dir;
}

char *get_eu_root_dir(){
    return eu_root_dir;
}

struct eu_file_desc *factory_file_desc(char *fname){
    struct eu_file_desc *newfile = (struct eu_file_desc *)memm_alloc(sizeof(struct eu_file_desc));
    newfile->lex_ctxt = lex_create_context();
    newfile->file_name = fname;
    if(strncmp(fname, EU_SHELL_FILE_NAME, sizeof(EU_SHELL_FILE_NAME) / (sizeof(char))) == 0){
        // shell mode. Setting input to 'stdin'
        newfile->fd = stdin;
    }else{
        // TODO:
        // need to perform some tests on the file before open it
        newfile->fd = fopen(fname, "r");
        if(newfile->fd == NULL){
            EUROPA_ERROR("Unable to open file '%s'\n", fname);
            return NULL;
        }
    }
    return newfile;
}