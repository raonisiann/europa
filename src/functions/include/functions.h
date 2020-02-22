
#ifndef I_FUNCTIONS_H
#define I_FUNCTIONS_H

#include "europa_types.h"
#include "reference.h"
#include "ast.h"
#include "expr.h"
#include "europa_debug.h"

#define BUILTIN_FUNC_ARGS struct list *args, struct e_context *ctxt


#define BUILTIN_FUNC_NAME(MOD, NAME) eu_func_##MOD##_##NAME
// Builtin function definition
// For the the builtin function "print" it will expand to: 
// struct e_value *eu_builtin_func_print(struct list *args, struct e_context *ctxt)
#define BUILTIN_FUNC_DEF(MOD, NAME) void BUILTIN_FUNC_NAME(MOD, NAME)(BUILTIN_FUNC_ARGS)

#define BUILTIN_LIB_ENTRY(NAME) { .lib = #NAME, .loader = eu_func_##NAME##_load }

// Builin load function 
#define BUILTIN_FUNC_LOAD(LIB) void eu_func_##LIB##_load()
#define BUILTIN_FUNC_METADATA_ENTRY(LIB, FUNC) { \
			.type = e_fbuiltin, \
			.name = #FUNC, \
			.eu_func_entry_ptr = eu_func_##LIB##_##FUNC \
		}

struct function_lib_metadata {
	const char *lib; 
	void (*loader) ();
};

// called during the lang initilization
void functions_init();
// load all functions from a lib
void functions_load_builtin(struct function_lib_metadata *lib);

//
// function list declaration
// 

#endif