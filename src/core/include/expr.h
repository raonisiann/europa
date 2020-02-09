#ifndef I_EXPR_H
#define I_EXPR_H

#include "europa_types.h"
#include "context.h"

//bool get_bool_value(struct ast_node *n);
char *get_string_value(struct ast_node *n);


//struct e_value *set_fcall_value(struct func_def *f);
struct e_value *set_token_type(char value);
struct e_value *do_arithmetic_sum(struct e_value *l, struct e_value *r);
struct e_value *do_arithmetic_subtract(struct e_value *l, struct e_value *r);
struct e_value *do_arithmetic_multiply(struct e_value *l, struct e_value *r);
struct e_value *do_arithmetic_division(struct e_value *l, struct e_value *r);
struct e_value *do_logical_equal(struct e_value *l, struct e_value *r);
struct e_value *do_logical_not_equal(struct e_value *l, struct e_value *r);
struct e_value *do_logical_and(struct e_value *l, struct e_value *r);
struct e_value *do_logical_or(struct e_value *l, struct e_value *r);
struct e_value *do_logical_greater_then(struct e_value *l, struct e_value *r);
struct e_value *do_logical_greater_then_or_equal(struct e_value *l, struct e_value *r);
struct e_value *do_logical_less_then(struct e_value *l, struct e_value *r);
struct e_value *do_logical_less_then_or_equal(struct e_value *l, struct e_value *r);
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


void value_eval(struct e_value *data);
struct e_value *expr_eval(struct ast_node *n, struct e_context *ctxt);

#endif