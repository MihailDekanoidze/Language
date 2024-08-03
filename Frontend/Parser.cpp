#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#include <ctype.h>

#include "../include/InputText.h"
#include "../include/Tree.h"
#include "../include/Read.h"
#include "../include/Differentiator.h"




#define REQUIRE(exp_symbol, token)                                              \
printf(RED "Expected symbol is %c, but curr_symbol is ", exp_symbol);           \
print_token_arg(token);                                                         \
printf("\n" RESET)

#define FUNC_LEN 4
#define CONSTANT_LEN 2


void parser_syntax_error(void)
{
    fprintf(stderr, "Syntax error\n");
}

Node* get_G(Token* token)
{                   
    CURR_TOKEN_PRINT(token);

    Node* node = get_A(&token);
 
    CURR_TOKEN_PRINT(token);
    
    if (token->token_type == t_end) {token++;} 
    else {REQUIRE('$', token);}
    return node;
}

Node* get_A(Token** token)
{
    CURR_TOKEN_PRINT(*token);

    Node* node1 = get_ASSIGN(token);

    CURR_TOKEN_PRINT(*token);

    if ((*token)->token_type == t_key_word && (*token)->val->key_word == act_end)
    {
        (*token)++;
        Node* node2 = get_A(token);

        node1  = _ACT_END(node1, node2);
    }
    return node1;
}

Node* get_ASSIGN(Token** token)
{
    CURR_TOKEN_PRINT(*token);

    Node* node1 = NULL;
    node1 = get_V(token);

    CURR_TOKEN_PRINT(*token);

    if ((*token)->token_type == t_op && (*token)->val->op == assign_op) 
    {
        (*token)++;
        
        Node* node2 = get_E(token);

        node1 = _ASSIGN(node1, node2);
    }
    return node1;
}

Node* get_E(Token** token)
{
    CURR_TOKEN_PRINT(*token);    
    
    Node* node1 = NULL;
    node1 = get_M(token);

    CURR_TOKEN_PRINT(*token);
    
    while   (((*token)->token_type == t_op)   &&
            (((*token)->val->op    == add_op) || ((*token)->val->op == sub_op)))
    {
        Operation op = (*token)->val->op;
        (*token)++;
        
        Node* node2 = get_M(token);

        if (op == add_op) node1 = _ADD(node1, node2);
        else              node1 = _SUB(node1, node2);
    }

    return node1;
}

Node* get_M(Token** token)
{
    CURR_TOKEN_PRINT(*token);
    
    Node* node1 = NULL;
    if (((*token)->token_type == t_op) && ((*token)->val->op == sub_op))
    {
        (*token)++;
        Node* node2 = get_T(token);

        CURR_TOKEN_PRINT(*token);        

        node1 = _NUM(-1);
        node1 = _MUL(node1, node2);
    }
    else
    {
        node1 = get_T(token);
    }

    return node1;
}

Node* get_T(Token** token)
{
    CURR_TOKEN_PRINT(*token);
    
    Node* node1 = get_D(token);
    
    CURR_TOKEN_PRINT(*token);
    
    while   (((*token)->token_type == t_op)   &&
            (((*token)->val->op    == mul_op) || ((*token)->val->op == divis_op)))
    {
        Operation op = (*token)->val->op;
        (*token)++;

        Node* node2 = get_D(token);

        if (op == mul_op)   node1 = _MUL(node1, node2);
        else                node1 = _DIV(node1, node2);
    }

    return node1;
}

Node* get_D(Token** token)
{
    CURR_TOKEN_PRINT(*token);

    Node* node1 = get_F(token);

    CURR_TOKEN_PRINT(*token);    

    while (((*token)->token_type == t_op) && (*token)->val->op == pow_op)
    {
        (*token)++;

        CURR_TOKEN_PRINT(*token);
        
        Node* node2 = get_D(token);

        node1 = _POW(node1, node2);
    }

    return node1;
}

Node* get_F(Token** token)
{
    CURR_TOKEN_PRINT(*token);   

    Node* node = NULL; 

    if ((*token)->token_type != t_func) node = get_P(token);
    else 
    {
        Function funct = (*token)->val->func;   
        (*token)++;

        if ((*token)->token_type != t_open_bracket) {REQUIRE('(', *token);}
        else
        {
            Node* arg = get_P(token);
            switch (funct)
            {
            case sin_f:
                node = _SIN(arg);
                break;
            case cos_f:
                node = _COS(arg);
                break; 
            case tg_f:
                node = _TG(arg);
                break;  
            case ctg_f:
                node = _DIV(_NUM(1), _TG(arg));
                break;
            case sh_f:
                node = _SH(arg);
                break;
            case ch_f:
                node = _CH(arg);
                break;
            case th_f:
                node = _TH(arg);
                break;
            case cth_f:
                node = _DIV(_NUM(1), _TH(arg));
                break;
            case ln_f:
                node = _LN(arg);
                break;
            case exp_f:
                node = _EXP(arg);
                break;
            case null_f:
            default:
                printf("Wrong func!\n");
                break;
            }
        }
    }

    return node;
}

Node* get_P(Token** token)
{  
    CURR_TOKEN_PRINT(*token);
    
    Node* node = NULL;


    if ((*token)->token_type != t_open_bracket) 
    {
        node = get_N(token);
    }
    else
    {
        (*token)++;
        node = get_E(token);
        
        CURR_TOKEN_PRINT(*token);

        if ((*token)->token_type != t_close_bracket) {REQUIRE(')', *token);}
        (*token)++;
    }

    return node;
}

Node* get_N(Token** token)
{
    CURR_TOKEN_PRINT(*token);
    
    Node* node = NULL;


    if ((*token)->token_type != t_number) 
    {
        node = get_V(token);
    }
    else
    {
        node =_NUM(((*token)->val->number));
        (*token)++;
    }

    return node;
}

Node* get_V(Token** token)
{
    CURR_TOKEN_PRINT(*token);
    
    Node* node = NULL;

    if ((*token)->token_type == t_var)
    {
        node = _VAR((*token)->val->var);
        (*token)++;
    }
    return node;
}




// add mono -
// add var

// 2^2^3 = 256 == 2^(2^3)
// -3*2 == -(3*2)