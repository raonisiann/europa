
#ifndef I_LEXER_H
#define I_LEXER_H

// Max buffer size used when capturing
#define LEX_CAP_BUFFER 1024

#define LEX_NULL_CHAR           '\0'
#define LEX_BELL_CHAR           '\a'
#define LEX_BACKSPACE           '\b'
#define LEX_HORIZONTAL_TAB      '\t'
#define LEX_NEW_LINE            '\n'
#define LEX_VERTICAL_TAB        '\v'
#define LEX_FORM_FEED           '\f'
#define LEX_CARRIAGE_RET        '\r'
#define LEX_WHITE_SPACE         ' '
#define LEX_DOUBLE_QUOTE        '"'
#define LEX_SINGLE_QUOTE        '\''
#define LEX_SCAPE_CHAR          '\\'

#define TOKEN lex_tk->class


typedef enum {
    undefined = -1,   
    eof = 0,
    newline, // \n 
    integer, 
    string,
    boolean, // 'true' or 'false'
    nullval, // null value  
    reference,
    plus, // +
    minus, // -
    multiply, // *
    division, // /
    assign, // = 
    negation, // !
    equal, // == 
    notequal, // not equal != 
    gt, // greater than >
    lt, // less than <
    gte, // greater than or equal >=
    lte, // less than or equal <=    
    andoper, // and operator 'and' 
    oroper, // or operator 'or'
    ocurlybrc, // open curly braces {
    ccurlybrc, // close curly braces }
    osqbrackets, // open square brackets [
    csqbrackets, // close square brackets ]
    oparenteses, // open parenteses (
    cparenteses, // close parenteses )
    colon, // colon : 
    comma,
    ifcmd,
    elsecmd, 
    whilecmd,
    defcmd,
    includecmd,
    fcall,
    parserexpr,
    returncmd, 
    parserfactor,
	usecmd
} lex_token_class;

// Struct to store a lex token 
// Must be filled with a class (from one of the above)
// and a value with the raw string captured (if necessary)
struct lex_token {
    lex_token_class class;
    unsigned int size; 
    unsigned int line_num;
    unsigned short end_pos; 
    char *raw_value;        
};

struct reserved_word {
    char *word;
    lex_token_class token;
};

// store the current lex token 
struct lex_token *lex_tk;


// Current char for lexer. It's updated by
// lex_next_char function
char lex_cur_ch;

char lex_prev_ch;

// Current char position (or column)
unsigned int lex_cur_ch_pos; 

// Current line number
unsigned int lex_cur_line;

//
// match functions
//
// Match function just return 1, when match
// and 0 if not match. To capture data from 
// input the "lex_cap_*" functions should 
// be used instead.  

// lex_match can be used to match a single char
// at the current lex position
int lex_match(const char expected);


// lex_match_optional can be used to match a 
// single char only. It's equivalent to "?" 
// for regex. It's also don't cause your program 
// to die if not match occur.
int lex_match_optional(const char match);

int lex_match_digit();
int lex_match_alpha();


void lex_next_token();
//
// capture functions
//
struct lex_token *lex_cap_digit();
struct lex_token *lex_cap_string();
struct lex_token *lex_cap_reference();

// create a lex token 
struct lex_token *lex_create_tk(int tk_class, unsigned int size, char *value);


// move forward the cursor to next available char
void lex_next_char();
void lex_unget_char();

//
// error/expection control functions
//

void lex_error(const char *error);

void lex_ignore_white_spaces();

void lex_clear_capture_buffer(char *buffer, unsigned int size);
void lex_init();

void lex_verbose(char *s, ...);

char *lex_token_to_text(int tk_class);

void lex_reset_state();

#endif