#ifndef I_REFERENCE_H
#define I_REFERENCE_H

#include "europa_types.h"
#include "ast.h"

struct e_reference *reference_factory();
struct e_value *reference_eval(struct ast_node *n);
void assignment_eval(struct ast_assignment_node *node);
void create_reference(char *name, struct e_value *v);

#endif