#ifndef I_REFERENCE_H
#define I_REFERENCE_H

#include "europa_types.h"
#include "ast.h"
#include "context.h"
#include "hashtable.h"
#include "europa/object.h"


struct e_value *function_eval(struct ast_node *fcall_node, struct e_context *ctxt);
void function_param_map(struct e_reference *fdef, struct ast_node *fcall_node, struct e_context *cur_ctxt, struct e_context *new_ctxt);
struct list *param_eval(struct ast_node *fcall_node, struct e_context *ctxt);
EuObject *reference_eval(struct ast_node *ref_node, struct e_context *ctxt);
void reference_assign_eval(struct e_assignment *assign, struct e_context *ctxt);
struct ht_entry *reference_lookup(char *name, struct e_context *ctxt);

#endif