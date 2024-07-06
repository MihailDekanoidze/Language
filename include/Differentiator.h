#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include "./include/CommonIncludes.h"
#include "./include/Token.h"
#include "./include/Tree.h"
#include "./include/DSL.h"
#include "./include/Colors.h"

//#define DEBUG_PRINTF

                 

#define PRINT_ARG(curr_node) if(curr_node->data_type == number) printf(" %lg \n", curr_node->val->number);         \
                                else printf(" %c \n", get_oper_symbol(curr_node->val->op))

#define _NUM(num) create_node(number, val_double(num), NULL, NULL)
#define _VAR(variable) create_node(var, val_var(variable), NULL, NULL)

#define _ADD(left_node, right_node)     create_node(operation, Add,     left_node, right_node)
#define _SUB(left_node, right_node)     create_node(operation, Sub,     left_node, right_node)
#define _MUL(left_node, right_node)     create_node(operation, Mul,     left_node, right_node)
#define _DIV(left_node, right_node)     create_node(operation, Div,     left_node, right_node)
#define _POW(left_node, right_node)     create_node(operation, Pow,     left_node, right_node)
#define _ASSIGN(left_node, right_node)  create_node(operation, Assign,  left_node, right_node)

#define _SIN(right_node) create_node(func, Sin, NULL, right_node)
#define _COS(right_node) create_node(func, Cos, NULL, right_node)
#define  _TG(right_node) create_node(func, Tg,  NULL, right_node)
#define _CTG(right_node) create_node(func, Ctg, NULL, right_node)
#define  _SH(right_node) create_node(func, Sh,  NULL, right_node)
#define  _CH(right_node) create_node(func, Ch,  NULL, right_node)
#define  _TH(right_node) create_node(func, Th,  NULL, right_node)
#define _CTH(right_node) create_node(func, Cth, NULL, right_node)
#define  _LN(right_node) create_node(func, Ln,  NULL, right_node)
#define _EXP(right_node) create_node(func, Exp, NULL, right_node)


#define T_NUM(num) create_token(number, num)
#define T_VAR(variab) create_token(var, variab)

#define T_ADD create_token(Operation, Add)
#define T_SUB create_token(Operation, Sub)
#define T_MUL create_token(Operation, Mul)
#define T_DIV create_token(Operation, Div)
#define T_POW create_token(Operation, Pow)

#define T_SIN create_token(Function, Sin)
#define T_COS create_token(Function, Cos)
#define  T_TG create_token(Function, Tg)
#define T_CTG create_token(Function, Ctg)
#define  T_SH create_token(Function, Sh)
#define  T_CH create_token(Function, Ch)
#define  T_TH create_token(Function, Th)
#define T_CTH create_token(Function, Cth)
#define  T_LN create_token(Function, Ln)

#define DL diff_the_tree(node->left)
#define DR diff_the_tree(node->right)
#define CN copy_node(node)
#define CL copy_node(node->left)
#define CR copy_node(node->right)

#define TAYLOR_MONOMIAL_LEN 10

Errors      file_read           (const char* file_name, Tree* tree);
Errors      file_write          (const char* file_name, Node* root);


size_t      nsymbol_in_str      (char* source, char symbol);
Token*      create_token        (Token_type data_type, node_data* val);

Node*       diff_the_tree       (const Node* node);
Node*       create_node         (Type, node_data*, Node*, Node*);
Node*       copy_node           (const Node* N);

int         nan_search           (Node* curr_node);
int         const_calculation    (Node* node, size_t* changes);
void        action_with_zero     (Node* node, size_t* changes);
void        action_with_one      (Node* node, size_t* changes);
void        tree_optimize        (Node* node);
double      tree_calc            (Node* N, double var_val);
void        var_substitution     (Node* N, double var_val);

size_t      factorial(size_t num);

Errors      Taylor_decomposition(const char* file_name, Tree* tree);

void        syntax_error(void);
Node*       get_G(Token* token_array);
Node*       get_N(Token** token);
Node*       get_E(Token** token);
Node*       get_T(Token** token);
Node*       get_P(Token** token);
Node*       get_F(Token** token);
Node*       get_D(Token** token);
Node*       get_M(Token** token);
Node*       get_V(Token** token);

//double     calc_get_G(const char* str, double var);
double     calc_get_N(void);
double     calc_get_E(void);
double     calc_get_T(void);
double     calc_get_P(void);
double     calc_get_C(void);
double     calc_get_F(void);
double     calc_get_D(void);

node_data* val_double           (double);
node_data* val_Operation        (Operation);
node_data* val_var              (char*); 
node_data* val_Function         (Function);

//int        Calculation(void);
void       parser_syntax_error(void);


#define Null    val_Operation(null_op)    // in main filling or use constructor
#define Add     val_Operation(add_op)
#define Sub     val_Operation(sub_op)     //array with funct node_datas
#define Mul     val_Operation(mul_op)
#define Div     val_Operation(divis_op)
#define Pow     val_Operation(pow_op)
#define Assign  val_Operation(assign_op)
#define Sin     val_Function(sin_f)
#define Cos     val_Function(cos_f)
#define Tg      val_Function(tg_f)
#define Ctg     val_Function(ctg_f)
#define Sh      val_Function(sh_f)
#define Ch      val_Function(ch_f)
#define Th      val_Function(th_f)
#define Cth     val_Function(cth_f)     
#define Ln      val_Function(ln_f)
#define Exp     val_Function(exp_f)

#endif