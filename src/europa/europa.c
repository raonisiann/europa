#include <string.h>
#include "europa.h"
#include "stack.h"
#include "memm.h"
#include "europa_error.h"

char europa_shell_mode = 0;
e_stack *eu_include_stack = NULL;
struct eu_file_desc *eu_current_include_file = NULL;

void eu_init(){

	eu_include_stack = stack_init(EU_INCLUDE_MAX_DEPTH);
}

void push_to_include_stack(struct eu_file_desc *file){
	
	stack_push(eu_include_stack, (void *)file);
	eu_current_include_file = file; 
}

struct eu_file_desc *pop_from_include_stack(){
	struct eu_file_desc *ret = NULL;
	ret = (struct eu_file_desc *)stack_pop(eu_include_stack);
	// update the current include file with 
	// the top of the stack 
	eu_current_include_file = (struct eu_file_desc *)stack_peek(eu_include_stack);
	return ret;
}

int get_include_stack_size(){
	
	return EU_INCLUDE_MAX_DEPTH - eu_include_stack->free; 
}

struct eu_file_desc *get_current_file_desc(){
	return eu_current_include_file;
}

struct eu_file_desc *factory_file_desc(char *fname){
	struct eu_file_desc *newfile = (struct eu_file_desc *)memm_alloc(sizeof(struct eu_file_desc));
	newfile->line_num = 0; 
	newfile->char_pos = 0; 
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

