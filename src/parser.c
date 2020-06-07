#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "statement.h"
#include "reference.h"
#include "flow.h"
#include "list.h"
#include "europa_debug.h"
#include "europa_error.h"
#include "europa.h"

extern struct lex_context *lex_cur_ctxt;

int parser_accept(int token){
    if(TOKEN_TYPE == token){
        DEBUG_OUTPUT("MATCHED TOKEN '%s'", TOKEN->raw_value);
        return 1;
    }
    return 0;
}

int parser_expect(int token){
    if(parser_accept(token)){
        return 1;
    }
    parser_error("Unexpected token", token);
    return 0;
}

// Parser Error raise an europa error which 
// could terminates the program
void parser_error(const char *error, int expected){
    EUROPA_ERROR(
        "Syntax Error: %s '%s' (%s) found at line %i, char %i. Expecting '%s' instead",
        error,
        TOKEN->raw_value, 
        lex_token_to_text(TOKEN->type),
        TOKEN->line_num,
        TOKEN->end_pos - TOKEN->size,
        lex_token_to_text(expected)
    );
}

//
// start the parsing of the lang 
//
struct e_stmt *top_level(){
    struct e_stmt *lang = NULL;
    if(parser_accept(newline)){
        return NULL;
    }else if(parser_accept(eof)){
        return NULL;
    }else if(parser_accept(defcmd)){
        // function definition
        // def FUNC_NAME '(' FUNC_ARGS ')' '{' FUNC_BODY '}'
        func_def();
        return NULL;
    }else if(parser_accept(usecmd)){
        struct lex_token *include_file_tk = NULL;
        // Include implementation
        // use 'file_path'
        lex_next_token();
        parser_expect(string);
        include_file_tk = TOKEN;
        lex_next_token();
        parser_expect(newline);
        // add file to stack and switch lex context
        eu_switch_file_context(include_file_tk->raw_value);
        // From now, new tokens will be retrieved from the opened file
        return NULL;
    }else{
        lang = stmt();
        return lang;
    }
}

void eu_lang(){
    struct e_stmt *stmt = NULL;
    while(1){
        lex_next_token();
        // end of file reached... ending program..
        if(parser_accept(eof) || TOKEN == NULL){
            break;
        }
        stmt = top_level();
        if(stmt != NULL){
            stmt_eval(stmt, GLOBAL_CTXT);
        }
    }
}

void eu_shell(){
    printf("Europa Language (%s) version %s\n", EU_BUILT_OS, EU_VERSION);
    printf("Built on %s %s\n", EU_BUILT_DATE, EU_DEBUG_INFO);
    printf("\n"
        "   _____                                \n"
        "  |  ___|                               \n"
        "  | |__ _   _ _ __ ___  _ __   __ _     \n"
        "  |  __| | | | '__/ _ \\| '_ \\ / _` |  \n"
        "  | |__| |_| | | | (_) | |_) | (_| |    \n"
        "  \\____/\\__,_|_|  \\___/| .__/ \\__,_|\n"
        "                       | |              \n"
        "                       |_|              \n"
        "\n"
        "\n"
    );

    struct e_stmt *stmt = NULL;
    struct list *results = NULL;
    while(1){
        printf("europa> ");
        // read
        lex_next_token();
        // parser instruction 
        stmt = top_level();
        if(stmt != NULL){
            // execute
            stmt_eval(stmt, GLOBAL_CTXT);
        }else{
            printf("\n");
        }
    }
}

void func_def(){
    struct e_reference *new_func_def = NULL;
    struct lex_token *ref_tk = NULL;
    struct list *arg_list = NULL;
    struct list *func_body = NULL;
    parser_expect(defcmd);
    lex_next_token();
    parser_expect(reference);
    ref_tk = TOKEN;
    lex_next_token();
    arg_list = get_func_arg_symbols();
    lex_next_token();
    func_body = stmt_block();
    // create and publish the reference to hashtable
    // We will get there...
    // hold on one sec function definitions.
    //set_ht_reference(new_fdef(ref_tk, arg_list, func_body), GLOBAL_CTXT);
}

struct e_stmt *stmt(){
    // NEW_LINE
    if(parser_accept(newline)){
        lex_next_token();
        return NULL;
    }
    // IF EXPR { BLOCK }
    else if(parser_accept(ifcmd)){
        struct ast_node* condition = NULL;
        struct list *if_block = NULL;
        struct list *else_block = NULL;
        lex_next_token();
        condition = expr();
        if_block = stmt_block();
        parser_ignore_new_lines();
        if(parser_accept(elsecmd)){
            lex_next_token();
            else_block = stmt_block();
        }
        return stmt_create_flow(s_if_flow, flow_create(condition, if_block, else_block));
    // WHILE EXPR { BLOCK }
    }else if(parser_accept(whilecmd)){
        struct ast_node* condition = NULL;
        struct list *while_block = NULL;
        lex_next_token(); 
        condition = expr();
        while_block = stmt_block();
        return stmt_create_flow(s_while_flow, flow_create(condition, while_block, NULL));
    }else if(parser_accept(returncmd)){
        // so far 'ret' with empty expression is not supported...
        // to use return you must to return something. 
        // Return ('ret') is followed by an expression ('expr()')
        // like 'ret 1', 'ret true', 'ret fcall() + 1'
        lex_next_token();
        return stmt_create_return(expr());
    }else{
    // EXPR, ASSINGMENT
        struct lex_token *tk_ref = NULL;
        if(parser_accept(reference)){
            tk_ref = TOKEN;
            lex_next_token();
            if(parser_accept(assign)){
                lex_next_token();
                return stmt_create_assign(tk_ref, expr());
            }
            // return token pointer to previous one.
            lex_prev_token();
        }
        return stmt_create_expr(expr());
    }
    return NULL;
}

// Accept and ignore new lines
// Could be used to ignore sequences of
// new lines that are not part of the program...
void parser_ignore_new_lines(){
    while(parser_accept(newline)){
        lex_next_token();
    }
}

struct list *stmt_block(){
    struct list *stmt_list = list_create();
    while(TOKEN_TYPE == newline){
        parser_expect(newline);
        lex_next_token();
    }
    if(parser_accept(ocurlybrc)){
        struct e_stmt *stmt_item = NULL;
        lex_next_token();
        while(TOKEN_TYPE != ccurlybrc){
            stmt_item = stmt();
            if(stmt_item){
                list_add_item(stmt_list, (void *)stmt_item);
            }
        }
        parser_expect(ccurlybrc);
        lex_next_token();
    }
    return stmt_list;
}

struct ast_node* expr(){
    struct ast_node* left = NULL;
    struct lex_token *tk = NULL; 
    left = term();
    while(TOKEN_TYPE == plus || TOKEN_TYPE == minus || TOKEN_TYPE == andoper || TOKEN_TYPE == oroper){
        tk = TOKEN;
        lex_next_token();
        left = ast_add_token_node(tk, left, term());
    }
    return left;
}

struct ast_node* term(){
    struct ast_node* fct = NULL;
    struct ast_node* parent = NULL;
    struct lex_token *tk = NULL;
    fct = factor();
    while(TOKEN_TYPE == multiply || TOKEN_TYPE == division || TOKEN_TYPE == equal || TOKEN_TYPE == notequal || TOKEN_TYPE == gt || TOKEN_TYPE == gte || TOKEN_TYPE == lt || TOKEN_TYPE == lte){
        tk = TOKEN;
        lex_next_token();
        parent = ast_add_token_node(tk, fct, term());
    }
    if(parent == NULL){
        parent = fct;
    }
    return parent;
}

struct ast_node* factor(){
    struct ast_node* leaf = NULL;
    if(parser_accept(object)){
        struct list *obj_args = list_create();
        list_add_item(obj_args, (void *)TOKEN->raw_value);
        EuObject *obj = object_create_instance(TOKEN->class_name, obj_args);
        leaf = ast_add_object_node(obj, NULL, NULL);
        lex_next_token();
    }else if(parser_accept(minus)){
        lex_next_token();
        parser_expect(integer);
        leaf = ast_add_value_node(token_to_neg_integer(TOKEN), NULL, NULL);
        lex_next_token();
    }else if(parser_accept(reference)){
        struct lex_token *ref_tk = TOKEN;
        lex_next_token();
        if(parser_accept(oparenteses)){
            leaf = ast_add_fcall_node(ref_tk, expr_list());
            parser_expect(cparenteses);
            lex_next_token();
        }else{
            leaf = ast_add_token_node(ref_tk, NULL, NULL);
        }
    }else if(parser_accept(oparenteses)){
        lex_next_token();
        leaf = expr();
        parser_expect(cparenteses);
        lex_next_token();
    }else if(parser_accept(osqbrackets)){
        lex_next_token();
    }else{
        parser_error("Unexpected token", parserexpr);
    }
    return leaf;
}

// Capture function argument symbols.
// A list of references inside parenteses separated by comma
// '(' a, b, c, var1, num, num2 ')'
struct list *get_func_arg_symbols(){
    parser_expect(oparenteses);
    lex_next_token();
    struct list *func_arg_list = list_create();
    if(parser_accept(cparenteses)){
        DEBUG_OUTPUT("No references captured captured");
    }else{
        struct e_stmt *arg_item = NULL;
        do{
            parser_expect(reference);
            arg_item = stmt_create_assign(TOKEN, NULL);
            list_add_item(func_arg_list, (void *)arg_item);
            lex_next_token();
            if(TOKEN_TYPE != comma){
                break;
            }
            parser_expect(comma);
            lex_next_token();
        }while(1);
    }
    parser_expect(cparenteses);
    return func_arg_list;
}

// Capture expression list.
// A list of expressions (expr) inside parenteses separated by comma
// '(' 11, funccall(), num, 1 + 2, false ')'
struct list *expr_list(){
    parser_expect(oparenteses);
    lex_next_token();
    struct list *l = list_create();
    if(parser_accept(cparenteses)){
    }else{
        struct ast_node *li = NULL;
        do{
            li = expr();
            list_add_item(l, (void *)li);
            if(TOKEN_TYPE != comma){
                break;
            }
            parser_expect(comma);
            lex_next_token();
        }while(1);
    }
    parser_expect(cparenteses);
    return l;
}