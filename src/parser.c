#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "statement.h"


extern struct lex_token *lex_tk;

// 
int parser_accept(int token){
    if(TOKEN == token){          
        parser_verbose("MATCHED TOKEN '%s'", lex_tk->raw_value);        
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


void parser_verbose(char *s, ...){
    va_list ap;
	va_start(ap, s);	
    printf("VERBOSE [PARSER] :: ");
	vfprintf(stdout, s, ap);
    printf("\n");	
    va_end(ap);    
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
            parser_verbose("Captured -> new line");    
            lex_next_token();                                         
        }else if(TOKEN == eof){
            parser_verbose("Captured -> End of file");        
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
        parser_verbose("new line for a statement");
        lex_next_token(); 
        return NULL;
    }
    // IF EXPR { BLOCK }
    else if (parser_accept(ifcmd)){            
        parser_verbose("IF");
        lex_next_token(); 
        expr();
        parser_verbose("EXPR");
        stmt_block();
        parser_verbose("END_IF");
    // WHILE EXPR { BLOCK }
    }else if (parser_accept(whilecmd)){            
        parser_verbose("WHILE");
        lex_next_token(); 
        expr();
        parser_verbose("EXPR");
        stmt_block();
        parser_verbose("END_WHILE");        
    }else{
    // EXPR, ASSINGMENT 
        struct ast_node* _expr = NULL;
        _expr = expr();         
        if(parser_accept(assign)){           
            struct ast_node* assign = NULL;
            parser_verbose("ASSIGNMENT");             
            lex_next_token(); 
            assign = expr();
            return stmt_create_assign(ast_create_assignment(_expr, assign));
        }else{
            return stmt_create_expr(_expr);
        }     
    }      
}

void stmt_block(){
    parser_verbose("IF_BLOCK");
    while(TOKEN == newline){
        parser_verbose("Ignoring empty new lines");
        parser_expect(newline);
        lex_next_token();
    }
    if(parser_accept(ocurlybrc)){
        lex_next_token();           
        while(TOKEN != ccurlybrc){
            stmt();                        
        }
        parser_expect(ccurlybrc);
        lex_next_token();
    }else{
        parser_verbose("Something else");
    }    
}

struct ast_node* expr(){   
    struct ast_node* left = NULL;
    struct ast_node* right = NULL;
    struct ast_node* parent = NULL;
    left = term();     
    if(TOKEN == plus || TOKEN == minus){                
        switch(TOKEN){
            case plus:             
                parser_expect(plus);                                               
                parent = ast_add_node(lex_tk, left, NULL);                                   
                break;
            case minus:                            
                parser_expect(minus);               
                parent = ast_add_node(lex_tk, left, NULL);                                   
                break;
        } 
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
    if(TOKEN == multiply || TOKEN == division){        
        switch(TOKEN){
            case multiply:
                parser_expect(multiply);
                parent = ast_add_node(lex_tk, left, NULL);   
                break;
            case division:
                parser_expect(division);
                parent = ast_add_node(lex_tk, left, NULL); 
                break;
        }
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
        parser_verbose("Captured -> digit (%i) %s", lex_tk->size, lex_tk->raw_value);         
        leaf = ast_add_node(lex_tk, NULL, NULL);
        lex_next_token();        
    }else if(parser_accept(string)){
        parser_verbose("Captured -> STRING (%i) %s", lex_tk->size, lex_tk->raw_value);     
        leaf = ast_add_node(lex_tk, NULL, NULL);    
        lex_next_token();
    }else if(parser_accept(reference)){
        parser_verbose("Captured -> reference (%i) %s", lex_tk->size, lex_tk->raw_value);
        leaf = ast_add_node(lex_tk, NULL, NULL);  
        lex_next_token();
        if(parser_accept(oparenteses)){            
            parser_verbose("FUNCTION CALL STARTED");
            lex_next_token();
            expr_list();
            parser_expect(cparenteses);
            parser_verbose("FUNCTION CALL ENDED");
            lex_next_token();            
            leaf = ast_add_node(lex_tk, NULL, NULL);        
        }
    }else if(parser_accept(oparenteses)){
            parser_verbose("OPEN_PARENTESES");
            lex_next_token();
            leaf = expr();
            parser_expect(cparenteses);
            parser_verbose("CLOSE_PARENTESES");
            lex_next_token();
    }else{
        parser_verbose("Syntax error: Cannot be -->> %s <<-- here", lex_tk->raw_value);
        lex_terminator();
    }   
    return leaf;     
}


void expr_list(){
    if(parser_accept(cparenteses)){
        parser_verbose("No arguments captured");
    }else{
        do{            
            parser_verbose("Capturing expression argument...");
            expr();               
            if(TOKEN != comma){
                break;
            }     
            parser_expect(comma);    
            lex_next_token();
        }while(1);  
    }  
}

// start point...
int parser_start(){

    lex_init();   
    lex_next_token();
    lang();   
    return 0;
}