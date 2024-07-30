#ifndef TOKEN_H
#define TOKEN_H

#include "./Colors.h"
#include "./CommonIncludes.h"
#include "./Tree.h"
#include "./InputText.h"


#ifdef DEBUG_ON
#define CURR_TOKEN_PRINT(token)             \
    getchar();                              \
    printf("Curr token is ");               \
    print_token_arg(token);                 \
    printf(" %s\n", __PRETTY_FUNCTION__);
#else
#define CURR_TOKEN_PRINT(token);            
#endif



enum Token_type
{
    t_empty,
    t_open_bracket,
    t_close_bracket,
    t_number,
    t_var,
    t_minus,
    t_func,
    t_op,
    t_end,
    t_key_word
};

union token_data
{
    char        symbol;
    char*       var;
    Operation   op;
    double      number;
    Function    func;
    Key_word    key_word;
};


typedef struct Token
{
    Token_type token_type;
    token_data * val;
}   Token;


text_info*  expression_tokenize(const text_info* expression);
double      get_number(const char* source);
void        op_search(const char* source, Token_type* type);
char*       get_var(const char* source);
void        token_array_print(const Token* token_array);
void        print_token_arg(const Token* token);
void        token_array_dtor(text_info* tokens);
char*       get_variable(const char* source);
#endif
