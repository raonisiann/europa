
#ifndef I_E_TYPES_H
#define I_E_TYPES_H
#include <stdio.h>
#include "lexer.h"
#include "europa_std.h"
#include "ast.h"
#include "list.h"
#include "context.h"

typedef enum {
    e_false = 0,
    e_true = 1
} e_bool;

typedef enum {
    e_undefined, 
    e_int,
    e_string, 
    e_boolean, 
    e_reference,
    e_fcall,
    e_fdef,
    e_fbuiltin
} e_type; 

struct e_array {
    struct list *items;
};

struct e_value {
    char type;     
    unsigned int size;     
    union {
        unsigned int boolean;  
        int num;                    
        char *str;                    
        struct e_array *arr;    
    };
};

struct e_fcall {    
    struct list *args;
    struct e_value *ret;
};

struct e_assignment {
    struct e_reference *ref; 
    struct ast_node *ast;
};

struct e_func_def {
    struct list *arg_list; // list of... 
    struct list *body; // list of e_stmt 
    struct e_context *ctxt; 
};

struct e_reference {
    char type;
    char *name;   
    union {
        struct e_value *value;       
        struct e_fcall *fcall;
        struct e_func_def *funcdef; 
        struct eu_func_builtin *bfunc;
        void (*eu_func_entry_ptr) (struct list *, struct e_context *);
    };
    unsigned int ht_hash;
};

struct e_assignment *factory_assigment();
struct e_value *factory_value();
struct e_value *token_to_integer(struct lex_token *token);
struct e_value *token_to_string(struct lex_token *token);
struct e_value *token_to_boolean(struct lex_token *token);
struct e_reference *factory_reference();
struct e_reference *new_reference(char *ref_name, struct e_value *val);
struct e_reference *new_fcall(struct e_reference *ref, struct list *args);
struct e_reference *new_fdef(struct lex_token *ref_tk, struct list *args, struct list *body);
struct e_fcall *factory_fcall();
struct e_func_def *factory_fdef();

#endif