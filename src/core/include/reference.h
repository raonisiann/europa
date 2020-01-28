#ifndef I_REFERENCE_H
#define I_REFERENCE_H

#include "europa_types.h"
#include "ast.h"


struct e_assignment *create_assignment(struct e_reference *ref, struct ast_node *n);
struct e_value *reference_eval(struct e_reference *ref);
void assignment_eval(struct e_assignment *assignment);
void create_reference(char *name, struct e_value *v);


#endif