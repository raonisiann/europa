#ifndef I_REFERENCE_H
#define I_REFERENCE_H

#include "europa_types.h"
#include "ast.h"


struct e_assignment *create_assignment(struct e_reference *ref, struct ast_node *n);
struct e_reference *create_reference();
struct e_value *reference_eval(struct ast_node *ref_node);
struct e_value *function_eval(struct ast_node *fcall_node);
void assignment_eval(struct lex_token *ref_tk, struct ast_node *expr);
void function_param_map(struct e_reference *fdef, struct ast_node *fcall_node);

void set_ht_reference(struct e_reference *ref);
struct e_reference *get_ht_reference(char *name);

void set_value_to_reference(struct e_reference *ref, struct e_value *v);


#endif