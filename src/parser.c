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
        }else if(TOKEN == eof){
            DEBUG_OUTPUT("Captured -> End of file");        
        }else{
            struct e_stmt *_stmt = NULL;
            _stmt = stmt();
            parser_expect(newline);               
            if(_stmt != NULL){
                // eval statement....
                stmt_eval(_stmt);
            }            
            lex_next_token();           
        } 
                      
    }    
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
        return stmt_create_flow(ifcmd, flow_create(condition, if_block, NULL));
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
        return stmt_create_flow(whilecmd, flow_create(condition, while_block, NULL));      
    }else{
    // EXPR, ASSINGMENT 
        struct ast_node* _expr = NULL;
        _expr = expr();         
        if(parser_accept(assign)){                       
            DEBUG_OUTPUT("ASSIGNMENT");             
            lex_next_token(); 
            if(_expr->type != node_reference){
                EUROPA_ERROR("An assign must use be of type 'reference'");
            }                        
            return stmt_create_assign(create_assignment(_expr->ref, expr()));
        }else{
            return stmt_create_expr(_expr);
        }     
    }      
}

struct list *stmt_block(){
    DEBUG_OUTPUT("IF_BLOCK");
    while(TOKEN == newline){
        DEBUG_OUTPUT("Ignoring empty new lines");
        parser_expect(newline);
        lex_next_token();        
    }
    if(parser_accept(ocurlybrc)){
        struct list *stmt_list = list_create();
        struct e_stmt *stmt_item = NULL;
        lex_next_token();           
        while(TOKEN != ccurlybrc){
            stmt_item = stmt();   
            if(stmt_item){
                list_add_item(stmt_list, (void *)stmt_item);
            }                     
        }
        parser_expect(ccurlybrc);
        lex_next_token();
        return stmt_list;
    }else{
        DEBUG_OUTPUT("Something else");
        return NULL;
    }    
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
        DEBUG_OUTPUT("Captured -> reference (%i) %s", lex_tk->size, lex_tk->raw_value);
        struct e_reference *new_ref = new_reference(lex_tk->raw_value, NULL);
        leaf = ast_add_reference_node(new_ref, NULL, NULL);  
        lex_next_token();
        if(parser_accept(oparenteses)){            
            DEBUG_OUTPUT("FUNCTION CALL STARTED");
            lex_next_token();
            new_ref->type = e_fcall;            
            new_ref->fcall->args = expr_list();
            parser_expect(cparenteses);
            DEBUG_OUTPUT("FUNCTION CALL ENDED");
            lex_next_token();                        
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


struct list *expr_list(){
    struct list *l = NULL;
    if(parser_accept(cparenteses)){
        DEBUG_OUTPUT("No arguments captured");        
    }else{      
        l = list_create();  
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
/*
void array_items_list(){
    if(parser_accept(csqbrackets)){
        DEBUG_OUTPUT("EMPTY_ARRAY");
    }else{
        struct ast_node *fac = NULL;
        struct ast_node *exp = NULL;
        do{            
            DEBUG_OUTPUT("Capturing array items...");
            fac = factor();
            if(TOKEN == colon){
                if(fac->token->class != integer && fac->token->class != string){
                    // raise error ... invalid array index (must be either integer or string)
                }
                expr(); 
            }
            expr();               
            if(TOKEN != comma){
                break;
            }     
            parser_expect(comma);    
            lex_next_token();
        }while(1);  
    }     
}

void array_values_list(){

}
*/

// start point...
int parser_start(){

    lex_init();   
    lex_next_token();
    lang();   
    return 0;
}