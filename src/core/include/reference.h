#ifndef I_REFERENCE_H
#define I_REFERENCE_H

#include "europa_types.h"
#include "ast.h"
#include "context.h"

struct e_reference *create_reference();
struct e_value *reference_eval(struct ast_node *ref_node, struct e_context *ctxt);
struct e_value *function_eval(struct ast_node *fcall_node, struct e_context *ctxt);
void assignment_eval(struct e_assignment *assign, struct e_context *ctxt);
void function_param_map(struct e_reference *fdef, struct ast_node *fcall_node, struct e_context *cur_ctxt, struct e_context *new_ctxt);
struct list *param_eval(struct ast_node *fcall_node, struct e_context *ctxt);
void set_ht_reference(struct e_reference *ref, struct e_context *ctxt);
struct e_reference *get_ht_reference(char *name, struct e_context *ctxt);
void assignment_new_reference(struct lex_token *tk_ref, struct e_value *value, struct e_context *ctxt);


#endif