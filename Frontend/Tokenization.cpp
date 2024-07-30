#include <stdio.h>
#include <strings.h>
#include "math.h"

#include "../include/CommonIncludes.h"
#include "../include/InputText.h"
#include "../include/Token.h"
#include "../include/ExtraFunctions.h"

text_info* expression_tokenize(const text_info* expression)
{
    printf_n_sym('*', STR_LEN);
    printf("\nExpression is \n\n%s\n \n", (char*)expression->buffer);
    printf_n_sym('*', STR_LEN);
    printf("\nExpression elemcount is %zu\n\n", expression->elemcount);
    
    text_info* token_array = text_info_ctor();
    token_array->buffer = calloc(expression->elemcount, sizeof(Token));
    Token* tokens =  (Token*)token_array->buffer;

    char* data = (char*)expression->buffer;
    size_t i = 0;
    Token_type token_type = t_empty;

    Operation oper = null_op;
    Function funct = null_f;
    Key_word word  = null_w;

    do
    {   
        #ifdef DEBUG_ON  
        printf("Current processing symbol is %c(%d)\n", *data, char_to_int(*data));
        #endif

        token_data* val = (token_data*)calloc(1, sizeof(node_data));
        size_t offset = 0;
        skip_spaces(&data);
        
        #ifdef DEBUG_ON
        printf("After skip_spaces processing symbol is %c(%d)\n", *data, char_to_int(*data));
        #endif

        if (*data == '(')           
        {
            token_type = t_open_bracket;
            val->symbol = *data;
            data++;
        }
        else if (*data == ')')
        {
            token_type = t_close_bracket;
            val->symbol = *data;
            data++;
        }           
        else if (*data == '$')
        {
           token_type = t_end;
           val->symbol = *data;
        }
        else 
        {
            oper = get_oper_code(data);
            if (oper)
            {
                token_type = t_op;
                val->op = oper;
                data++;
            }
            else
            {
                funct = get_funct_code(data);

                if (funct)
                {
                    token_type = t_func;
                    val->func = funct;
                    data += strlen(get_funct_name(funct));
                }
                else
                {
                    word = get_key_word_number(data);

                    if (word)
                    {
                        token_type = t_key_word;
                        val->key_word = word;
                        data += strlen(get_key_word_str(word));

                        #ifdef DEBUG_ON
                        printf("Token type is key word\n");
                        printf("val is %s\n", get_key_word_str(val->key_word));
                        printf("number of keyword is %zu\n", (size_t)word);
                        
                        #endif
                    }
                    else if (isalpha(*data)) 
                    {
                        token_type = t_var;
                        val->var = get_variable(data);
                        data += strlen(val->var);
                    }
                    else
                    {
                        token_type = t_number;
                        double num = 0;
                        if (sscanf(data, "%lg", &num))
                        {
                            #ifdef DEBUG_ON
                            printf("number is %lg\n", num);
                            #endif

                            val->number = num;
                            int point = 0;
                            while (isdigit(*data) || (*data == '.'))
                            {
                                if (*data == '.')
                                {
                                    if (!point) point++;
                                    else
                                    {
                                        printf("Wrong number is %s\n", data);
                                        return NULL;
                                    }
                                }
                                data++;
                            }
                        }
                        else {printf("Unknown token is %c\nRemained str is %s\n", *data, data);}
                    }
                }  
            }   
        }
        (tokens + i)->token_type = token_type;
        (tokens + i)->val= val;

        #ifdef DEBUG_ON
        print_token_arg(tokens + i);
        printf("\n");
        printf("Token number is %zu (i = %zu)\n\n", i + 1, i);

        printf("Remained str is %s\n", data);

        #endif
        

        i++;
    }   while(token_type != t_end);
    token_array->elemcount = i;

    return token_array;
}

void op_search(const char* source, Token_type* type)
{
    if (get_oper_code(source)) *type = t_op;
}

char* get_var(const char* source)
{
    char* buffer = (char*)calloc(STR_LEN, sizeof(char));
    size_t i = 0;
    while (isalpha(*source))
    {
        buffer[i++] = *source;
        source++;
    }
    return buffer;
}

double get_number(const char* source)
{   
    double val = 0;
    sscanf(source, "%lg", &val);
    int point = 0;
    while (isdigit(*source) || (*source == '.'))
    {
        if (*source == '.')
        {
            if (!point) point++;
            else
            {
                printf("Wrong number is %s\n", source);
                return NAN;
            }
        }
        source++;
    }
    return val; 
}


void token_array_print(const Token* token_array)
{
    if (!token_array)
    {
        printf("Token_array has null_ptr!\n");
        return;
    }
    size_t i = 0;

    while(token_array[i].token_type != t_end)
    {
        printf("Token_array[%zu] type is %d\n",i,  token_array[i].token_type);
        printf("val is ");
        print_token_arg(token_array + i);
        printf("\n");
        i++;
    }
    printf("Token_array[%zu] type is %d\n",i,  token_array[i].token_type);
    printf("val is ");
    print_token_arg(token_array + i);
    printf("\n");
}

void print_token_arg(const Token* token)
{
    printf("curr_token is ");
    switch (token->token_type)
    {                                       
    case t_number:
        printf(" #%lg# ", token->val->number);
        break;
    case t_var:
        printf(" #%s# ", token->val->var);
        break;
    case t_op:
        printf(" #%c# ", get_oper_symbol(token->val->op));
        break;
    case t_func:
        fprint_func(stdout, token->val->func);
        break;
    case t_empty:
        printf("Empty token\n");
        break;
    case t_open_bracket:
    case t_close_bracket:
    case t_minus:
    case t_end:
        printf(" #%c# ", token->val->symbol);
        break;
    case t_key_word:
        printf(" #%s# ", get_key_word_str(token->val->key_word));
        break;
    default:
        printf("Unknown token type <%lln>\n", (long long*)(token->val));
        break;
    }
}

void token_array_dtor(text_info* tokens)
{
    for (size_t i = 0; i < tokens->elemcount; i++)
    {
        Token* curr_token = ((Token*)(tokens->buffer) + i);
        if (curr_token->token_type == t_var) free(curr_token->val->var);
        free(curr_token->val);
    }
    text_info_dtor(tokens);
}

char* get_variable(const char* source)
{
    size_t len = BASE_VAR_LEN;
    char* var = (char*)calloc(len, sizeof(char));
    size_t var_len = 0;

    while (isalpha(*source))
    {
        if (len >= var_len + 1)
        {
            var = (char*)realloc(var, len * 2);
            len *= 2;
        }

        var[var_len] = *source;
        var_len++;
        source++; 
    }
    var[var_len] = '\0';

    return var;
}




