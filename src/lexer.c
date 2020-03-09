#include <stdio.h> // printf, stdin
#include <stdlib.h> // malloc, exit
#include <ctype.h> // isalpha, isdigit
#include <string.h> // memcpy, strndup
#include <stdarg.h>
#include "lexer.h"
#include "memm.h"
#include "europa_std.h"
#include "europa_debug.h"
#include "europa_error.h"
#include "europa.h"

struct lex_token *lex_tk;
extern struct eu_file_desc *eu_current_include_file;

struct reserved_word lex_reserved_words[10][7] = {
    // Size 0 commands
    { 
        {NULL, undefined}
    }, 
    // Size 1 commands 
    { 
        {NULL, undefined}
    },
    // Size 2 comamnds
    {
        {"if", ifcmd},
        {"or", oroper},
        {NULL, undefined}
    },
    // Size 3 comamnds
    {
        {"def", defcmd},
        {"ret", returncmd},
        {"and", andoper},
		{"use", usecmd},
        {NULL, undefined}
    },
    // Size 4 comamnds
    {
        {"else", elsecmd},
        {"true", boolean},
        {"null", nullval},
        {NULL, undefined}
    },
    // Size 5 comamnds
    {
        {"while", whilecmd},
        {"false", boolean},
        {NULL, undefined}
    },
    // Size 6 commands 
    { 
        {NULL, undefined}
    },       
    // Size 7 commands 
    { 
        {"include", includecmd},
        {NULL, undefined}
    },         
    // Size 8 commands 
    { 
        {NULL, undefined}
    },       
    // Size 9 commands 
    {         
        {NULL, undefined}
    }    
};

char lex_token_text[39][27] = {   
	"EOF", 
    "new line",
    "integer", 
    "string",
    "boolean", // 'true' or 'false'
    "null", 
    "reference",
    "plus", // +
    "minus", // -
    "multiply", // *
    "division", // /
    "assign", // = 
    "negation", // !
    "equal", // == 
    "not equal", // not equal != 
    "greater than", // greater than >
    "less than", // less than <
    "greater than or equal", // greater than or equal >=
    "less than or equal", // less than or equal <=    
    "'and' operator", // and operator 'and' 
    "'or' operator", // or operator 'or'
    "open curly braces", // open curly braces {
    "close curly braces", // close curly braces }
    "open square brackets", // open square brackets [
    "close square brackets", // close square brackets ]
    "open parenteses", // open parenteses (
    "close parenteses", // close parenteses )
    "colon", // colon : 
    "comma",
    "if statement",
    "else statement", 
    "while statement",
    "def",
    "include",
    "function call",
    "expression",
    "return",
    "factor",
	"use"
};

// Exactly match the "match"
// with lex_cur_ch. 
int lex_match(const char expected){        
    if(expected != lex_cur_ch){
        lex_error("Unexpected character");        
    }
    DEBUG_OUTPUT("Matched  -> %c\n", expected);    
    return 1;	
}

// Match optional char at lex_cur_ch 
int lex_match_optional(const char match){
    return 1;
}


void lex_next_token(){
    DEBUG_OUTPUT("NEXT TOKEN");
    // grab the next available char   
    lex_next_char();
    // this will end up with a non-space char, 
    // so don't need to call lex_next_char one more time
    lex_ignore_white_spaces();  
    
    if(feof(CURRENT_FD)){
		// check if the included file stack 
		//size is 1... if true, the program will be 
		// terminated, otherwise the file will be pop out 
		// from the stack and this token will be ignored. 		
		if(get_include_stack_size() == 1){
			lex_tk = lex_create_tk(eof, 11, "END_OF_FILE");    
			return;
		}else{
			// pop out the included file from stack
			pop_from_include_stack();
			lex_tk = NULL;			
			lex_cur_ch = '\0';
			lex_prev_ch = '\0';  	
			// request from previous file
			lex_next_char();	
			lex_ignore_white_spaces();  			
		}        
    }
	
	if(lex_cur_ch == '#'){
        while(lex_cur_ch != '\n'){
            lex_next_char();
        }		
		CURRENT_LINE_NUM++;
		CURRENT_CHAR_POS = 0;        	
    }else if(isdigit(lex_cur_ch)){
        // start digit/number capture         
        lex_tk = lex_cap_digit();
    }else if(isalpha(lex_cur_ch)){        
        // start identifier/keyword capture         
        lex_tk = lex_cap_reference();    
    }else{
        switch(lex_cur_ch){                
            case '\n':
                // new line capture                 
                lex_tk = lex_create_tk(newline, 8, "NEW_LINE");   
				CURRENT_LINE_NUM++;
				CURRENT_CHAR_POS = 0;
                break;
            case '\r':
                lex_next_char();
                if(lex_cur_ch == '\n'){
                    // new line capture                 
                    lex_tk = lex_create_tk(newline, 8, "NEW_LINE");   
					CURRENT_LINE_NUM++;
					CURRENT_CHAR_POS = 0;    
                    break;                  
                }else{
                    lex_error("Unable to handler non-newline after return. Expected '\\r\\n'.");
                }
            case '"':
                // start string capture   
                lex_tk = lex_cap_string(); 
                break;
            case '+':                
                lex_tk = lex_create_tk(plus, 1, "+");                   
                break;
            case '-':
                lex_tk = lex_create_tk(minus, 1, "-");       
                break;
            case '*':
                lex_tk = lex_create_tk(multiply, 1, "*");       
                break;
            case '/':
                lex_tk = lex_create_tk(division, 1, "/");       
                break;
            case '=':                   
                lex_next_char();
                if(lex_cur_ch == '='){
                    lex_tk = lex_create_tk(equal, 2, "==");
                }else{
                    lex_tk = lex_create_tk(assign, 1, "="); 
                    lex_unget_char();
                }                   
                break;
            case '!':                   
                lex_next_char();
                if(lex_cur_ch == '='){
                    lex_tk = lex_create_tk(notequal, 2, "!=");
                }else{
                    lex_tk = lex_create_tk(negation, 1, "!"); 
                    lex_unget_char();
                }                   
                break;                
            case '{':
                lex_tk = lex_create_tk(ocurlybrc, 1, "{");
                break;
            case '}':
                lex_tk = lex_create_tk(ccurlybrc, 1, "}");
                break;
            case '[':
                lex_tk = lex_create_tk(osqbrackets, 1, "[");
                break;
            case ']':
                lex_tk = lex_create_tk(csqbrackets, 1, "]");
                break;     
            case ':':
                lex_tk = lex_create_tk(colon, 1, ":");
                break;                            
            case '(':
                lex_tk = lex_create_tk(oparenteses, 1, "(");
                break;
            case ')':
                lex_tk = lex_create_tk(cparenteses, 1, ")");
                break;                
            case ',':
                lex_tk = lex_create_tk(comma, 1, ",");
                break;  
            case '<':                
                lex_next_char();
                if(lex_cur_ch == '='){
                    lex_tk = lex_create_tk(lte, 2, "<=");
                }else{
                    lex_tk = lex_create_tk(lt, 1, "<");
                    lex_unget_char();
                }
                break;                                  
            case '>':                
                lex_next_char();
                if(lex_cur_ch == '='){
                    lex_tk = lex_create_tk(gte, 2, ">=");
                }else{
                    lex_tk = lex_create_tk(gt, 1, ">");
                    lex_unget_char();
                }                
                break;                                             
            default: 				
                lex_error("Unable to handle character");
        }        
    }       
}

// capture digits [0-9]
struct lex_token *lex_cap_digit(){
    char buf[LEX_CAP_BUFFER];
    unsigned int buf_size = 0;       
    while(isdigit(lex_cur_ch)){
        buf[buf_size] = lex_cur_ch;
        lex_next_char();
        buf_size++;
    }
    // ending buffer
    buf[buf_size] = '\0';    
    lex_unget_char();
    // if nothing has been captured raise an error
    if(buf_size == 0){
        lex_error("Fail to capture a digit");
    }    
    return lex_create_tk(integer, buf_size, buf);
}

// Capture strings (char sequences inside quotes).
// Quotes are not part of the string, they are
// just demiliters on where the char sequence starts
// or ends.
struct lex_token *lex_cap_string(){
    char *string_captured = memm_alloc(sizeof(char));	
    char buf[LEX_CAP_BUFFER];
    unsigned int string_size = 0;
    unsigned int buf_size = 0;
	// initialize string with null terminator
	string_captured[0] = '\0';
    lex_next_char(); // ignoring opening quotes
    while(1){
        if (buf_size >= LEX_CAP_BUFFER){			
			string_captured = memm_realloc(string_captured, sizeof(char) * (string_size + 1));
			string_captured = strncat(string_captured, buf, buf_size);			
            buf_size = 0;
        }
        if(lex_cur_ch == LEX_DOUBLE_QUOTE){
            lex_next_char();			
            break;
        }
        if(lex_cur_ch == LEX_SCAPE_CHAR){
            lex_next_char();
            switch(lex_cur_ch){
                case 'a':
                    buf[buf_size] = LEX_BELL_CHAR;
                break;                  
                case 'b':
                    buf[buf_size] = LEX_BACKSPACE;
                break;                  
                case 't':
                    buf[buf_size] = LEX_HORIZONTAL_TAB;
                break;                                                                  
                case 'n':
                    buf[buf_size] = LEX_NEW_LINE;
                break;      
                case 'v':
                    buf[buf_size] = LEX_VERTICAL_TAB;
                break;                  
                case 'f':
                    buf[buf_size] = LEX_FORM_FEED;
                break;                  
                case 'r':
                    buf[buf_size] = LEX_CARRIAGE_RET;
                break;   
                case '"':
                    buf[buf_size] = LEX_DOUBLE_QUOTE;
                break; 
                case '\\':
                    buf[buf_size] = LEX_SCAPE_CHAR;
                break;                                                                                            
                default: 
                    lex_error("Invalid scape char");          
            }            
        }else{
            buf[buf_size] = lex_cur_ch;
        }        
        lex_next_char();
        buf_size++;
        string_size++;
    }    
    if(string_size > LEX_CAP_BUFFER || string_size != 0){		
        string_captured = memm_realloc(string_captured, sizeof(char) * (string_size + 1));
        strncat(string_captured, buf, buf_size);        		
    }
    lex_unget_char();        
    return lex_create_tk(string, string_size, string_captured);
}

// capture references
struct lex_token *lex_cap_reference(){
    char buf[LEX_CAP_BUFFER];
    unsigned int buf_size = 0;       

    // first char must be an alpha [a-zA-Z]
    if(isalpha(lex_cur_ch)){
        buf[buf_size] = lex_cur_ch;
        lex_next_char();
        buf_size++;
    }
    // Then it's allowed to have numbers, 
    // underscore (_), dash (-)
    // [a-zA-z_-]
    while(isalpha(lex_cur_ch) || 
            isdigit(lex_cur_ch) || 
            lex_cur_ch == '-' ||
            lex_cur_ch == '_'){
        buf[buf_size] = lex_cur_ch;
        lex_next_char();
        buf_size++;
    }
    // ending buffer
    buf[buf_size] = '\0';
    lex_unget_char();    
    // if nothing has been captured raise an error
    // unlikely... 
    if(buf_size == 0){
        lex_error("Fail to capture a reference");
    }    
    if(buf_size < 10){
        int k = 0;
        int match = 1;
        while(lex_reserved_words[buf_size][k].word != NULL){
            match = 1;
            for(int i = 0; i < buf_size; i++){
                if(lex_reserved_words[buf_size][k].word[i] != buf[i]){
                    match = 0;
                    break;
                }
            }
            if(match){
                DEBUG_OUTPUT("Captured -> Reserved word (%i) %s", buf_size, lex_reserved_words[buf_size][k].word);
                return lex_create_tk(lex_reserved_words[buf_size][k].token, buf_size, buf);   
            }
            k++;
        }
    }
    return lex_create_tk(reference, buf_size, buf);       
}

// Create a lex token 
struct lex_token *lex_create_tk(int class, unsigned int size, char *value){	
    struct lex_token *tk = memm_alloc(sizeof(struct lex_token));
    tk->class = class;
    if(size > 0){
        tk->size = size;        
        tk->raw_value = _STRING_DUP(value);
        tk->line_num = CURRENT_LINE_NUM;
        tk->end_pos = CURRENT_CHAR_POS;
    }else{
        tk->size = 0;
        tk->raw_value = NULL;   
        tk->line_num = 0;
        tk->end_pos = 0;     
    }    
    return tk;
}

// Ignore white spaces 
void lex_ignore_white_spaces(){        
    while(lex_cur_ch == LEX_WHITE_SPACE || lex_cur_ch == LEX_HORIZONTAL_TAB){        
        lex_next_char();
    }                
}

// Move forward the cursor to next available char
void lex_next_char(){          
    lex_prev_ch = lex_cur_ch; 
    lex_cur_ch = getc(CURRENT_FD);
    CURRENT_CHAR_POS++;     
}

// Move backward the cursor to previous stored char
void lex_unget_char(){    
    ungetc(lex_cur_ch, CURRENT_FD);
    lex_cur_ch = lex_prev_ch;    
    CURRENT_CHAR_POS--;
}

// Lex error function... 
void lex_error(const char *error){   
	// save lex state
	unsigned int ln = CURRENT_LINE_NUM;
	unsigned int chpos = CURRENT_CHAR_POS;
	char cur_char = lex_cur_ch;
	// discard all remain line and reset char and line counters
	while(lex_cur_ch != '\n'){
		lex_cur_ch = getc(CURRENT_FD);
	}
	CURRENT_CHAR_POS = 1;
	CURRENT_LINE_NUM = 1;
    EUROPA_ERROR("%s: Found '%c' at line %i, char %i", 
        error,
        cur_char, 
        ln, 
        chpos
    );                
}

void lex_clear_capture_buffer(char *buffer, unsigned int size){
    for(int i = 0; i < size; i++){
        buffer[i] = '\0';
    }
}

char *lex_token_to_text(int tk_class){
	if(tk_class < 0){
		return "Undefined"; 
	}
    return lex_token_text[tk_class];
}

// Reset lex settings to default values
void lex_reset_state(){
    lex_tk = NULL;    
    lex_cur_ch = '\0';
    lex_prev_ch = '\0';
    CURRENT_CHAR_POS = 1;
    CURRENT_LINE_NUM = 1;  	
}

// Initialize lexer settings
void lex_init(){

	lex_reset_state();
}
