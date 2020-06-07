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

#define TOKEN lex_cur_ctxt->tk_list->cur->tk
#define TOKEN_TYPE TOKEN->type
#define LEX_CUR_CHAR lex_cur_ctxt->lex_cur_ch
#define LEX_PREV_CHAR lex_cur_ctxt->lex_prev_ch
#define LEX_LINE_NUM lex_cur_ctxt->line_num
#define LEX_CHAR_POS lex_cur_ctxt->char_pos

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
    usecmd,
    object
} lex_tk_type;

struct tk_list {
    int size;
    struct tk_node *first;
    struct tk_node *last;
    struct tk_node *cur;
};

struct tk_node {
    struct tk_node *next;
    struct tk_node *prev;
    struct lex_token *tk;
};

// lex_tk : tk_list->cur->tk
struct lex_context {
    char lex_cur_ch;
    char lex_prev_ch;
    unsigned int line_num;
    unsigned int char_pos;
    struct tk_list *tk_list;
};

// Struct to store a lex token 
// Must be filled with a type (from one of the above)
// and a value with the raw string captured (if necessary)
struct lex_token {
    lex_tk_type type;
    unsigned int size;
    unsigned int line_num;
    unsigned short end_pos;
    char *raw_value;
    char *class_name;
};

struct reserved_word {
    char *word;
    char *class;
    lex_tk_type token;
};

// current lex context
struct lex_context *lex_cur_ctxt;

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
void lex_prev_token();
//
// capture functions
//
struct lex_token *lex_cap_digit();
struct lex_token *lex_cap_string();
struct lex_token *lex_cap_reference();

// create a lex token
struct lex_token *lex_create_tk(int tk_type, unsigned int size, char *value, char *class_name);

// move forward the cursor to next available char
void lex_next_char();
void lex_unget_char();

//
// error/expection control functions
//
void lex_error(const char *error);
void lex_ignore_white_spaces();
void lex_clear_capture_buffer(char *buffer, unsigned int size);
void lex_verbose(char *s, ...);
char *lex_token_to_text(int tk_type);
void lex_reset_state();
void lex_switch_context(struct lex_context *ctxt);
struct lex_context *lex_create_context();
struct tk_list *tk_create_list();
struct tk_node *tk_create_node();
void tk_add_node(struct tk_list *list, struct lex_token *tk);

#endif