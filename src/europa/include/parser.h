
#ifndef I_PARSER_H
#define I_PARSER_H

#include "list.h"

int parser_accept(int token);
int parser_expect(int token);

int parser_start();

void parser_error_expected(char *s, ...);

void eu_shell();
void eu_lang();
struct e_stmt *top_level();
struct e_stmt *stmt();
struct ast_node* expr();
struct ast_node* term();
struct ast_node* factor();
// statement(s) inside the '{' STMT_BLOCK '}'
struct list *stmt_block();
// expressions separated by ','
struct list *expr_list();
struct list *get_func_arg_symbols();
void func_def();
void parser_ignore_new_lines();

#endif