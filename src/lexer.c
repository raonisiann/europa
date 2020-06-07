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

extern struct eu_file_desc *eu_current_include_file;

static struct reserved_word lex_reserved_words[10][7] = {
    // Size 0 commands
    { 
        {NULL, NULL, undefined}
    }, 
    // Size 1 commands
    { 
        {NULL, NULL, undefined}
    },
    // Size 2 comamnds
    {
        {"if", NULL, ifcmd},
        {"or", NULL, oroper},
        {NULL, NULL, undefined}
    },
    // Size 3 comamnds
    {
        {"def", NULL, defcmd},
        {"ret", NULL, returncmd},
        {"and", NULL, andoper},
        {"use", NULL, usecmd},
        {NULL, NULL, undefined}
    },
    // Size 4 comamnds
    {
        {"else", NULL, elsecmd},
        {"true", "Boolean", object},
        {"null", "Null", object},
        {NULL, NULL, undefined}
    },
    // Size 5 comamnds
    {
        {"while", NULL, whilecmd},
        {"false", "Boolean", object},
        {NULL, NULL, undefined}
    },
    // Size 6 commands
    {
        {NULL, NULL, undefined}
    },
    // Size 7 commands
    {
        {"include", NULL, includecmd},
        {NULL, NULL, undefined}
    },
    // Size 8 commands
    {
        {NULL, NULL, undefined}
    },
    // Size 9 commands
    {
        {NULL, NULL, undefined}
    }
};

char *lex_token_text[] = {
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
    "use",
    "object"
};

// Exactly match the "match"
// with LEX_CUR_CHAR.
int lex_match(const char expected){
    if(expected != LEX_CUR_CHAR){
        lex_error("Unexpected character");
    }
    DEBUG_OUTPUT("Matched  -> %c\n", expected);
    return 1;
}

// Match optional char at LEX_CUR_CHAR 
int lex_match_optional(const char match){
    return 1;
}

void lex_next_token(){
    struct tk_list *list = NULL;
    list = lex_cur_ctxt->tk_list;
    if(list->cur != NULL && list->cur->next != NULL){
        list->cur = list->cur->next;
        return;
    }

    struct lex_token *lex_tk = NULL;
    DEBUG_OUTPUT("NEXT TOKEN");
    // check if the next on the list is null.
    // If not advance the cursor to the next.
    // Otherwise start capturing a new token

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
            lex_tk = lex_create_tk(eof, 11, "END_OF_FILE", NULL);
            tk_add_node(lex_cur_ctxt->tk_list, lex_tk);
        }else{
            // pop out the included file from stack
            pop_from_include_stack();
            lex_next_token();
        }
        return;
    }

    if(LEX_CUR_CHAR == '#'){
        while(LEX_CUR_CHAR != '\n'){
            lex_next_char();
        }
        LEX_LINE_NUM++;
        LEX_CHAR_POS = 0;
    }else if(isdigit(LEX_CUR_CHAR)){
        // start digit/number capture
        lex_tk = lex_cap_digit();
    }else if(isalpha(LEX_CUR_CHAR)){
        // start identifier/keyword capture
        lex_tk = lex_cap_reference();
    }else{
        switch(LEX_CUR_CHAR){
            case '\n':
                // new line capture
                lex_tk = lex_create_tk(newline, 8, "NEW_LINE", NULL);
                LEX_LINE_NUM++;
                LEX_CHAR_POS = 0;
                break;
            case '\r':
                lex_next_char();
                if(LEX_CUR_CHAR == '\n'){
                    // new line capture
                    lex_tk = lex_create_tk(newline, 8, "NEW_LINE", NULL);
                    LEX_LINE_NUM++;
                    LEX_CHAR_POS = 0;
                    break;
                }else{
                    lex_error("Unable to handler non-newline after return. Expected '\\r\\n'.");
                }
            case '"':
                // start string capture
                lex_tk = lex_cap_string();
                break;
            case '+':
                lex_tk = lex_create_tk(plus, 1, "+", NULL);
                break;
            case '-':
                lex_tk = lex_create_tk(minus, 1, "-", NULL);
                break;
            case '*':
                lex_tk = lex_create_tk(multiply, 1, "*", NULL);
                break;
            case '/':
                lex_tk = lex_create_tk(division, 1, "/", NULL);
                break;
            case '=':
                lex_next_char();
                if(LEX_CUR_CHAR == '='){
                    lex_tk = lex_create_tk(equal, 2, "==", NULL);
                }else{
                    lex_tk = lex_create_tk(assign, 1, "=", NULL);
                    lex_unget_char();
                }
                break;
            case '!':
                lex_next_char();
                if(LEX_CUR_CHAR == '='){
                    lex_tk = lex_create_tk(notequal, 2, "!=", NULL);
                }else{
                    lex_tk = lex_create_tk(negation, 1, "!", NULL);
                    lex_unget_char();
                }
                break;
            case '{':
                lex_tk = lex_create_tk(ocurlybrc, 1, "{", NULL);
                break;
            case '}':
                lex_tk = lex_create_tk(ccurlybrc, 1, "}", NULL);
                break;
            case '[':
                lex_tk = lex_create_tk(osqbrackets, 1, "[", NULL);
                break;
            case ']':
                lex_tk = lex_create_tk(csqbrackets, 1, "]", NULL);
                break;
            case ':':
                lex_tk = lex_create_tk(colon, 1, ":", NULL);
                break;
            case '(':
                lex_tk = lex_create_tk(oparenteses, 1, "(", NULL);
                break;
            case ')':
                lex_tk = lex_create_tk(cparenteses, 1, ")", NULL);
                break;
            case ',':
                lex_tk = lex_create_tk(comma, 1, ",", NULL);
                break;
            case '<':
                lex_next_char();
                if(LEX_CUR_CHAR == '='){
                    lex_tk = lex_create_tk(lte, 2, "<=", NULL);
                }else{
                    lex_tk = lex_create_tk(lt, 1, "<", NULL);
                    lex_unget_char();
                }
                break;
            case '>':
                lex_next_char();
                if(LEX_CUR_CHAR == '='){
                    lex_tk = lex_create_tk(gte, 2, ">=", NULL);
                }else{
                    lex_tk = lex_create_tk(gt, 1, ">", NULL);
                    lex_unget_char();
                }
                break;
            default:
                lex_error("Unable to handle character");
        }
    }
    // add token to the list
    tk_add_node(lex_cur_ctxt->tk_list, lex_tk);
}

// Move the token cursor backward on the list
void lex_prev_token(){
    struct tk_list *list = NULL;
    list = lex_cur_ctxt->tk_list;
    if(list->cur == NULL || list->cur->prev == NULL){
        return;
    }	
    list->cur = list->cur->prev;
}

// capture digits [0-9]
struct lex_token *lex_cap_digit(){
    char buf[LEX_CAP_BUFFER];
    unsigned int buf_size = 0;
    while(isdigit(LEX_CUR_CHAR)){
        buf[buf_size] = LEX_CUR_CHAR;
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
    return lex_create_tk(object, buf_size, buf, "Integer");
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
        if(LEX_CUR_CHAR == LEX_DOUBLE_QUOTE){
            lex_next_char();
            break;
        }
        if(LEX_CUR_CHAR == LEX_SCAPE_CHAR){
            lex_next_char();
            switch(LEX_CUR_CHAR){
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
            buf[buf_size] = LEX_CUR_CHAR;
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
    return lex_create_tk(object, string_size, string_captured, "String");
}

// capture references
struct lex_token *lex_cap_reference(){
    char buf[LEX_CAP_BUFFER];
    unsigned int buf_size = 0;
    // first char must be an alpha [a-zA-Z]
    if(isalpha(LEX_CUR_CHAR)){
        buf[buf_size] = LEX_CUR_CHAR;
        lex_next_char();
        buf_size++;
    }
    // Then it's allowed to have numbers,
    // underscore (_), dash (-)
    // [a-zA-z_-]
    while(isalpha(LEX_CUR_CHAR) ||
            isdigit(LEX_CUR_CHAR) ||
            LEX_CUR_CHAR == '-' ||
            LEX_CUR_CHAR == '_'){
        buf[buf_size] = LEX_CUR_CHAR;
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
                return lex_create_tk(lex_reserved_words[buf_size][k].token, buf_size, buf, lex_reserved_words[buf_size][k].class);
            }
            k++;
        }
    }
    return lex_create_tk(reference, buf_size, buf, NULL);
}

// Create a lex token 
struct lex_token *lex_create_tk(int type, unsigned int size, char *value, char *class_name){
    struct lex_token *tk = memm_alloc(sizeof(struct lex_token));
    tk->type = type;
    if(size > 0){
        tk->size = size;
        tk->raw_value = _STRING_DUP(value);
        tk->line_num = LEX_LINE_NUM;
        tk->end_pos = LEX_CHAR_POS;
        tk->class_name = class_name;
        return tk;
    }
    return NULL;
}

// Ignore white spaces
void lex_ignore_white_spaces(){
    while(LEX_CUR_CHAR == LEX_WHITE_SPACE || LEX_CUR_CHAR == LEX_HORIZONTAL_TAB){
        lex_next_char();
    }
}

// Move forward the cursor to next available char
void lex_next_char(){
    LEX_PREV_CHAR = LEX_CUR_CHAR;
    LEX_CUR_CHAR = getc(CURRENT_FD);
    LEX_CHAR_POS++;
}

// Move backward the cursor to previous stored char
void lex_unget_char(){
    ungetc(LEX_CUR_CHAR, CURRENT_FD);
    LEX_CUR_CHAR = LEX_PREV_CHAR;
    LEX_CHAR_POS--;
}

// Lex error function
void lex_error(const char *error){
    // save lex state
    unsigned int ln = LEX_LINE_NUM;
    unsigned int chpos = LEX_CHAR_POS;
    char cur_char = LEX_CUR_CHAR;
    // discard all remain line and reset char and line counters
    while(LEX_CUR_CHAR != '\n'){
    LEX_CUR_CHAR = getc(CURRENT_FD);
    }
    LEX_CHAR_POS = 1;
    LEX_LINE_NUM = 1;
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

char *lex_token_to_text(int tk_type){
    if(tk_type < 0){
        return "Undefined";
    }
    return lex_token_text[tk_type];
}

// Reset lex settings to default values
void lex_reset_state(){
    LEX_CUR_CHAR = '\0';
    LEX_PREV_CHAR = '\0';
    LEX_CHAR_POS = 1;
    LEX_LINE_NUM = 1;
}


struct lex_context *lex_create_context(){
    struct lex_context *ctxt = (struct lex_context *)memm_alloc(sizeof(struct lex_context));
    ctxt->lex_cur_ch = '\0';
    ctxt->lex_prev_ch = '\0';
    ctxt->line_num = 1;
    ctxt->char_pos = 1;
    ctxt->tk_list = tk_create_list();
    return ctxt;
}

struct tk_list *tk_create_list(){
    struct tk_list *tk_list = (struct tk_list *)memm_alloc(sizeof(struct tk_list));
    tk_list->size = 0;
    tk_list->first = NULL;
    tk_list->last = NULL;
    tk_list->cur = NULL;
    return tk_list;
};

struct tk_node *tk_create_node(){
    struct tk_node *node = (struct tk_node *)memm_alloc(sizeof(struct tk_node));
    node->next = NULL;
    node->prev = NULL;
    node->tk = NULL;
    return node;
};

void tk_add_node(struct tk_list *list, struct lex_token *tk){
    struct tk_node *new = tk_create_node();
    new->tk = tk;
    if(list->first == NULL){
        list->first = new;
    }else{
        list->last->next = new;
    new->prev = list->last;
    }

    list->last = new;
    // adding a new token makes the new to be the current token
    list->cur = new;
    list->size++; 
}

void lex_switch_context(struct lex_context *ctxt){
    lex_cur_ctxt = ctxt;
}