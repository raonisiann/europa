#ifndef I_EXPR_H
#define I_EXPR_H

#include "europa_types.h"
#include "context.h"
#include "europa/object.h"

//bool get_bool_value(struct ast_node *n);
char *get_string_value(struct ast_node *n);


int do_string_compare(char *str1, char *str2);
int do_number_compare(int n1, int n2);
char *do_string_concat(struct e_value *str1, struct e_value *str2);
char *convert_to_string_value(struct e_value *v);
e_bool is_string_equal(char *str1, char *str2);
e_bool is_number_equal(int n1, int n2);
e_bool is_bool_equal(int b1, int b2);
e_bool is_string_not_equal(char *str1, char *str2);
e_bool is_number_not_equal(int n1, int n2);
e_bool is_bool_not_equal(int b1, int b2);


void value_eval(EuObject *data);
void return_eval(struct ast_node *n, struct e_context *ctxt);
EuObject *expr_eval(struct ast_node *n, struct e_context *ctxt);
EuObject *set_token_type(char value);
EuObject *expr_sum(EuObject *a, EuObject *b);
EuObject *expr_subtract(EuObject *l, EuObject *r);
EuObject *expr_multiply(EuObject *l, EuObject *r);
EuObject *expr_division(EuObject *l, EuObject *r);
EuObject *expr_equal(EuObject *l, EuObject *r);
EuObject *expr_not_equal(EuObject *l, EuObject *r);
EuObject *expr_and(EuObject *l, EuObject *r);
EuObject *expr_or(EuObject *l, EuObject *r);
EuObject *expr_greater_then(EuObject *l, EuObject *r);
EuObject *expr_greater_then_or_equal(EuObject *l, EuObject *r);
EuObject *expr_less_then(EuObject *l, EuObject *r);
EuObject *expr_less_then_or_equal(EuObject *l, EuObject *r);

#endif