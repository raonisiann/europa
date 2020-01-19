
#ifndef I_E_TYPES_H
#define I_E_TYPES_H
#include <stdio.h>
#include "lexer.h"
#include "europa_std.h"
#include "ast.h"

typedef enum {
    false = 0,
    true = 1
} bool;

// User defined function at rutime
// This same struct is used to store a function call (user or built-in)
// If is a function call header and body will be null
/*
typedef struct europa_func_def {
    char name[SYM_MAX_SIZE]; 
    struct dl_list *args_list; 
    unsigned int args_count;    
    struct dl_list *body;
    struct i_data *ret_value;
    void (*func_ptr) (struct func_def *, struct eval_context *);
} eur_function_def;
*/

struct e_value {
    char type;     
    unsigned int size;     
    union {
        char *str;            
        int num;    
        bool boolean;                 
    };
};


struct e_reference {
    char *name;     
    struct e_value *value; 
    unsigned int ht_hash;
};


struct e_flow {
    char type;
    struct ast_node *condition;
    //struct dl_list *iftrue;    
    //struct dl_list *iffalse;    
};


struct e_value *factory_value();
struct e_value *create_int_value(struct lex_token *token); 
struct e_value *create_boolean_value(struct lex_token *token);
struct e_value *create_string_value(struct lex_token *token);
struct e_value *create_reference_value(struct lex_token *token);

#endif