
#ifndef I_PARSER_H
#define I_PARSER_H


int parser_accept(int token);
int parser_expect(int token);

int parser_start();

void parser_error_expected(char *s, ...);
void parser_verbose(char *s, ...);

void lang();
struct e_stmt *stmt();
struct ast_node* expr();
struct ast_node* term();
struct ast_node* factor();
// statement(s) inside the '{' STMT_BLOCK '}'
void stmt_block();
// expressions separated by ','
void expr_list();

#endif