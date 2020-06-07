#include "reference.h"
#include "expr.h"
#include "hashtable.h"
#include "memm.h"
#include "europa_error.h"
#include "europa_debug.h"
#include "statement.h"
#include "context.h"
#include "stack.h"
#include "europa/object.h"

// function call param map should evaluate
// input variables (params) in the current context
// then create new ones in the new context.
// 'cur_context' (current) and 'new_context' can
// be the same if function is being called in
// global context, or different is the call is
// made from another function
/*
void function_param_map(struct e_reference *fdef, struct ast_node *fcall_node, struct e_context *cur_ctxt, struct e_context *new_ctxt){
    struct list_item *input_arg = NULL, *def_arg = NULL;
    EuObject *input_value = NULL;
    struct e_assignment *assign = NULL;
    if(fdef->funcdef->arg_list->size != fcall_node->fcall->expr_list->size){
        EUROPA_ERROR("Wrong number of parameters for function '%s', \
        is expecting %i, but you provided %i\n",
        fcall_node->token->raw_value,
        fdef->funcdef->arg_list->size,
        fcall_node->fcall->expr_list->size);
    }

    input_arg = fcall_node->fcall->expr_list->first;
    def_arg = fdef->funcdef->arg_list->first;
    while(input_arg != NULL){
        DEBUG_OUTPUT("INPUT_ARG_EXPR_EVAL");
        assign = ((struct e_stmt *)def_arg->data)->assign;
        input_value = expr_eval((struct ast_node *)input_arg->data, cur_ctxt);
        ht_set_key(assign->ref, input_value, new_ctxt);
        DEBUG_OUTPUT("NEXT_ITEM");
        input_arg = input_arg->next;
        def_arg = def_arg->next;
    }
}
*/

/*
// Run evaluation of all parameters in the list
// and return a new list of e_value
struct list *param_eval(struct ast_node *fcall_node, struct e_context *ctxt){
    struct list_item *input_arg = NULL;
    // value after expression eval
    EuObject *res_value = NULL;
    struct list *res_param_list = NULL;
    res_param_list = list_create();
    // get first parameter
    input_arg = fcall_node->fcall->expr_list->first;
    while(input_arg != NULL){
        DEBUG_OUTPUT("INPUT_ARG_EXPR_EVAL");
        res_value = expr_eval((struct ast_node *)input_arg->data, ctxt);
        list_add_item(res_param_list, res_value);
        input_arg = input_arg->next;
    }
    return res_param_list;
}
*/

/*
struct e_value *function_eval(struct ast_node *fcall_node, struct e_context *ctxt){
    DEBUG_OUTPUT("FUNC_EVAL_STARTING");
    struct e_reference *fdef = NULL;
    struct e_context *fcall_ctxt = NULL;
    EuObject *ret_value = NULL;
    struct list *input_list_res = NULL;
    // function eval are placed on global table by default
    fdef = get_ht_reference(fcall_node->fcall->func->raw_value, GLOBAL_CTXT);
    if(fdef){
        fcall_ctxt = context_create(15);
        if(fdef->type == e_fbuiltin){
            input_list_res = param_eval(fcall_node, ctxt);
            fdef->eu_func_entry_ptr(input_list_res, fcall_ctxt);
        }else{
            function_param_map(fdef, fcall_node, ctxt, fcall_ctxt);
            stmt_block_eval(fdef->funcdef->body, fcall_ctxt);
            DEBUG_OUTPUT("FUNC_EVAL_END");
        }
        ret_value = fcall_ctxt->ret_val;
        context_destroy(fcall_ctxt);
        return ret_value;
    }else{
        EUROPA_ERROR("Reference for function '%s' not found\n", fcall_node->fcall->func->raw_value);
        return ret_value;
    }
}
*/

EuObject *reference_eval(struct ast_node *ref_node, struct e_context *ctxt){
    struct ht_entry *result = reference_lookup(ref_node->token->raw_value, ctxt);
    if(result){
        return result->object;
    }
    EUROPA_ERROR("Reference '%s' not found", ref_node->token->raw_value);
    return NULL;
}

void reference_assign_eval(struct e_assignment *assign, struct e_context *ctxt){
    EuObject *expr_value = NULL;
    struct ht_entry *entry = NULL;
    expr_value = expr_eval(assign->ast, ctxt);
    entry = reference_lookup(assign->ref->raw_value, ctxt);
    if(entry){
    // re-assign
    // does the previous referenced object will be lost in memory?
    // garbage collected?
        entry->object = expr_value;
    }else{
        // new reference
        ht_set_key(ctxt->symbols, assign->ref->raw_value, expr_value);
    }
}

// Return an entry from the hashtable
//
// Resolution order:
// 1. Look at the local context at 'ctxt' if defined
// 2. If found on local, return the value
// 3. If not found on local, look at the global context
// 4. If not found on global, neither local, return NULL
struct ht_entry *reference_lookup(char *name, struct e_context *ctxt){
    struct ht_entry *entry = NULL;
    if(ctxt != NULL){
        entry = ht_get(ctxt->symbols, name);
        if(entry != NULL){
            return entry;
        }
    }
    // look at global
    entry = ht_get(GLOBAL_CTXT->symbols, name);
    if(entry != NULL){
        return entry;
    }
    return NULL;
}