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

extern struct lex_token *lex_tk;

// 
int parser_accept(int token){
    if(TOKEN == token){          
        DEBUG_OUTPUT("MATCHED TOKEN '%s'", lex_tk->raw_value);        
        return 1;
    }
    return 0;
}

// 
int parser_expect(int token){
    if(parser_accept(token)){
        return 1;
    }
    parser_error_expected("Unexpected token '%s', should be -->>%i<<--\n", lex_tk->raw_value, token);
}

// 
void parser_error_expected(char *s, ...){
    va_list ap;
	va_start(ap, s);	
	vfprintf(stderr, s, ap);	
    va_end(ap);
    exit(-1);
}

//
// start the parsing of the lang 
//
void lang(){        
    while(1){        
        if(parser_accept(newline)){
            DEBUG_OUTPUT("Captured -> new line");                         
            lex_next_token();                                         
        }else if(parser_accept(eof)){
            DEBUG_OUTPUT("Captured -> End of file");   
            break;
        }else if(parser_accept(defcmd)){     
            // function definition
            // def FUNC_NAME '(' FUNC_ARGS ')' '{' FUNC_BODY '}'  
            DEBUG_OUTPUT("STARTING FUNCTION_DEFINITION");                      
            func_def();
            DEBUG_OUTPUT("ENDING FUNCTION_DEFINITION");
            //lex_next_token();
        }else{
            struct e_stmt *_stmt = NULL;
            _stmt = stmt();            
            parser_expect(newline);               
            DEBUG_OUTPUT("EVALUATING STATEMENT");
            if(_stmt != NULL){
                // eval statement....
                stmt_eval(_stmt);
            }            
            lex_next_token();           
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
    ref_tk = lex_tk;    
    lex_next_token();    
    arg_list = get_func_arg_symbols();    
    lex_next_token();    
    func_body = stmt_block();    
    // create and publish the reference to hashtable
    set_ht_reference(new_fdef(ref_tk, arg_list, func_body));    
}

struct e_stmt *stmt(){    
    // NEW_LINE
    if(parser_accept(newline)){
        DEBUG_OUTPUT("new line for a statement");            
        lex_next_token(); 
        return NULL;
    }
    // IF EXPR { BLOCK }
    else if (parser_accept(ifcmd)){   
        struct ast_node* condition = NULL;         
        struct list *if_block = NULL;
        DEBUG_OUTPUT("IF");
        lex_next_token(); 
        condition = expr();
        DEBUG_OUTPUT("EXPR");
        if_block = stmt_block();
        DEBUG_OUTPUT("END_IF");
        return stmt_create_flow(s_if_flow, flow_create(condition, if_block, NULL));
    // WHILE EXPR { BLOCK }
    }else if (parser_accept(whilecmd)){    
        struct ast_node* condition = NULL;         
        struct list *while_block = NULL;                
        DEBUG_OUTPUT("WHILE");
        lex_next_token(); 
        condition = expr();
        DEBUG_OUTPUT("EXPR");
        while_block = stmt_block();
        DEBUG_OUTPUT("END_WHILE"); 
        return stmt_create_flow(s_while_flow, flow_create(condition, while_block, NULL));      
    }else{
    // EXPR, ASSINGMENT 
        DEBUG_OUTPUT("EXPR");
        struct ast_node* _expr = NULL;
        _expr = expr();         
        if(parser_accept(assign)){                       
            DEBUG_OUTPUT("ASSIGNMENT");             
            lex_next_token(); 
            if(_expr->type != node_assign){
                EUROPA_ERROR("An assign must use be of type 'reference'");
            }                        
            _expr->assign->expr = expr();
            return stmt_create_assign(_expr);
        }else{
            return stmt_create_expr(_expr);
        }     
    }      
}

struct list *stmt_block(){
    DEBUG_OUTPUT("IF_BLOCK");
    struct list *stmt_list = list_create();
    while(TOKEN == newline){
        DEBUG_OUTPUT("Ignoring empty new lines");
        parser_expect(newline);
        lex_next_token();        
    }
    if(parser_accept(ocurlybrc)){
        DEBUG_OUTPUT("OPEN_CURLY_BRACES");
        struct e_stmt *stmt_item = NULL;
        lex_next_token();           
        while(TOKEN != ccurlybrc){
            stmt_item = stmt();   
            if(stmt_item){
                list_add_item(stmt_list, (void *)stmt_item);
            }                     
        }
        parser_expect(ccurlybrc);
        DEBUG_OUTPUT("CLOSE_CURLY_BRACES");
        lex_next_token();        
    }else{
        DEBUG_OUTPUT("Something else");      
    } 
    return stmt_list;
}

struct ast_node* expr(){   
    struct ast_node* left = NULL;
    struct ast_node* right = NULL;
    struct ast_node* parent = NULL;
    left = term();     
    if(TOKEN == plus || TOKEN == minus || TOKEN == andoper || TOKEN == oroper){                
        switch(TOKEN){
            case plus:             
                parser_expect(plus);                                                                                                  
                break;
            case minus:                            
                parser_expect(minus);                                                                  
                break;
            case andoper:                            
                parser_expect(andoper);                                                                  
                break;
            case oroper:                            
                parser_expect(oroper);                                                                  
                break;                                
        } 
        parent = ast_add_token_node(lex_tk, left, NULL);
        lex_next_token();
        right = expr();            
        parent->right = right;
        return parent;
    }    
    return left;    
}


struct ast_node* term(){   
    struct ast_node* left = NULL;
    struct ast_node* right = NULL;   
    struct ast_node* parent = NULL;  
    left = factor();
    if(TOKEN == multiply || TOKEN == division || TOKEN == equal || TOKEN == notequal || TOKEN == gt || TOKEN == gte || TOKEN == lt || TOKEN == lte){        
        switch(TOKEN){
            case multiply:
                parser_expect(multiply);                
                break;
            case division:
                parser_expect(division);                
                break;
            case equal:
                parser_expect(equal);                
                break;                
            case notequal:
                parser_expect(notequal);                
                break;                  
            case gt: 
                parser_expect(gt);                
                break;                                  
            case gte: 
                parser_expect(gte);                
                break;                          
            case lt: 
                parser_expect(lt);                
                break;                                  
            case lte: 
                parser_expect(lte);                
                break;                                          
        }
        parent = ast_add_token_node(lex_tk, left, NULL); 
        lex_next_token();
        right = term();   
        parent->right = right;  
        return parent;        
    }  
    return left;  
}


struct ast_node* factor(){   
    struct ast_node* leaf = NULL;   
    if(parser_accept(integer)){
        DEBUG_OUTPUT("Captured -> digit (%i) %s", lex_tk->size, lex_tk->raw_value);                 
        leaf = ast_add_value_node(token_to_integer(lex_tk),  NULL, NULL);
        lex_next_token();        
    }else if(parser_accept(string)){
        DEBUG_OUTPUT("Captured -> STRING (%i) %s", lex_tk->size, lex_tk->raw_value);     
        leaf = ast_add_value_node(token_to_string(lex_tk),  NULL, NULL);           
        lex_next_token();
    }else if(parser_accept(boolean)){
        DEBUG_OUTPUT("Captured -> BOOLEAN (%i) %s", lex_tk->size, lex_tk->raw_value);               
        leaf = ast_add_value_node(token_to_boolean(lex_tk),  NULL, NULL); 
        lex_next_token();        
    }else if(parser_accept(reference)){    
        DEBUG_OUTPUT("REFERENCE");
        struct lex_token *ref_tk = lex_tk;         
        lex_next_token();      
        // if is an assigment return immediately to statement function
        if(parser_accept(assign)){              
            return ast_add_assign_node(ref_tk, NULL);                       
        }else if(parser_accept(oparenteses)){            
            DEBUG_OUTPUT("FUNCTION CALL STARTED");
            lex_next_token();            
            leaf = ast_add_fcall_node(ref_tk, expr_list());            
            parser_expect(cparenteses);
            DEBUG_OUTPUT("FUNCTION CALL ENDED");
            lex_next_token();                        
        }else{
            leaf = ast_add_token_node(ref_tk, NULL, NULL);    
        }
    }else if(parser_accept(oparenteses)){
        DEBUG_OUTPUT("OPEN_PARENTESES");
        lex_next_token();
        leaf = expr();
        parser_expect(cparenteses);
        DEBUG_OUTPUT("CLOSE_PARENTESES");
        lex_next_token();
    }else if(parser_accept(osqbrackets)){
        DEBUG_OUTPUT("OPEN_SQUARE_BRACKETS");
        lex_next_token();
    }else{
        DEBUG_OUTPUT("Syntax error: Cannot be -->> %s <<-- here", lex_tk->raw_value);
        lex_terminator();
    }   
    return leaf;     
}

// Capture function argument symbols.
// A list of references inside parenteses separated by comma
// '(' a, b, c, var1, num, num2 ')'
struct list *get_func_arg_symbols(){
    struct list *func_arg_list = list_create();
    if(parser_accept(cparenteses)){
        DEBUG_OUTPUT("No references captured captured");                      
    }else{
        parser_expect(oparenteses);
        lex_next_token();
        struct ast_node *arg_item = NULL;        
        do{            
            DEBUG_OUTPUT("Capturing reference list...");
            parser_expect(reference);
            // TODO: 
            // no need for ast here... ?
            arg_item = ast_add_token_node(lex_tk, NULL, NULL);                              
            list_add_item(func_arg_list, (void *)arg_item);
            lex_next_token();           
            if(TOKEN != comma){
                break;
            }     
            parser_expect(comma);    
            lex_next_token();
        }while(1);      
    }  
    parser_expect(cparenteses);
    DEBUG_OUTPUT("MATCHING CLOSE_PARENTESES");
    return func_arg_list;
}

struct list *expr_list(){
    struct list *l = list_create();
    if(parser_accept(cparenteses)){
        DEBUG_OUTPUT("No arguments captured");        
    }else{              
        struct ast_node *li = NULL;
        do{            
            DEBUG_OUTPUT("Capturing expression argument...");
            li = expr();               
            list_add_item(l, (void *)li);
            if(TOKEN != comma){                
                break;
            }     
            parser_expect(comma);    
            lex_next_token();
        }while(1);  
    }  
    return l;
}

// start point...
int parser_start(){

    lex_init();   
    lex_next_token();
    lang();   
    return 0;
}