#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#include "./CommonIncludes.h"


#define BASE_NODE_COUNT 100
#define TREE_NODE_INCREASE_COEF   20     
#define STR_LEN         100
#define OBJECT_FOUND    1 << 7
#define OBJECT_N_FOUND  1 << 8
#define RUS_CHAR_SIZE   sizeof('а')
#define OPERATIONS_AND_FUNCTIONS_COUNT 7
#define TREE_CALLOC_ERROR  NULL;
#define OPERATION_COUNT 5
#define FUNCTION_COUNT 10
#define BASE_VAR_LEN   5

#define TREE_PRINT(node_list, file)     size_t level = 0;                                    \
                                        tree_print(node_list, file, &level)


#define NODE_CHILD_PRINT(file, node, child)             fprintf(file, "->\"");                      \
                                                        fprint_arg(file, node);                     \
                                                        fprintf(file, "\n\n%p\"", node);               \
                                                        node_dot_create(child, file)                

#define START_NODE_CHILD_PRINT(file, node, child)       fprintf(file, "\"");                        \
                                                        fprint_arg(file, node);                     \
                                                        fprintf(file, "\n\n%p\"", node);               \
                                                        node_dot_create(child, file)

#define FOPEN(file_name, mode) fopen(file_name, mode); fprintf(stderr, "open: %s on line: %d %s\n", #file_name, __LINE__, __PRETTY_FUNCTION__);
#define FCLOSE(file_name) int file_close = fclose(file_name); fprintf(stderr, "close: %s  on line: %d %s\n", #file_name, __LINE__, __PRETTY_FUNCTION__); \
    if (file_close == 0) {fprintf(stderr, "meow!!\n");}


typedef unsigned char Data;
typedef struct Node Node;

enum Operation
{
    null_op ,
    add_op  ,
    sub_op  ,
    divis_op,
    mul_op  ,
    pow_op  ,
    assign_op
};

enum Function
{
    null_f,
    sin_f ,
    cos_f ,
    ln_f  ,
    tg_f  ,
    ctg_f ,
    sh_f  ,
    ch_f  ,
    th_f  ,
    cth_f ,
    exp_f   
};

typedef union node_data
{
    char* var;
    Operation op;
    double number;
    Function func;
} node_data; 


enum Type
{
    empty_node = 0,
    operation  = 1,
    number     = 2,
    var        = 3,
    func       = 4
};


typedef struct Node {
    Type data_type;
    node_data* val;            
    Node* left;
    Node* right;
    Node* previous;
} Node;

enum Child
{
    left  = 1,
    right = 2
};

struct Tree {
    Node* root;
    FILE* tree_log;
};

Tree*               tree_create(void);
void                node_print(const Node* tree, FILE* tree_data);
void                tree_detor(Tree* tree);
void                node_dtor_all(Node* node);
void                node_dtor_one(Node* node, Child);

Operation           get_oper_code       (char* source);
Function            get_funct_code      (char* func);
char                get_oper_symbol     (Operation);
const char*         get_funct_name      (Function funct);
        
Node*               tree_add_node(Node* parent, Child subtree, Type tp, void* arg);

void                skip_spaces(char* source, size_t* pos);
void                skip_alpha(char* source, size_t* pos);
void                ClearBuffer(void);
void                fprint_nchar(FILE* dest, char symbol, size_t count);


void                fprint_arg(FILE* dest, const Node* curr_node);
void                fprint_func(FILE* dest, Function func);
void                print_curr_node(const Node* curr_node);


//struct Stack*       way_stack(Tree* tree, char* val);
//int                 way_search(Node* curr_node, double val, struct Stack* way_to_obj);

Errors              graph_image(Node* start, const char* name);
void                node_dot_create(Node* curr_node, FILE* tree_info);

struct Operation_info
{
    Operation op;
    const char symbol;
};

struct Function_info
{
    Function function;
    const char name[10];
}; 

const Operation_info op_info[] =
{
    {add_op, '+'},
    {sub_op, '-'},
    {mul_op, '*'},
    {divis_op, '/'},
    {pow_op, '^'},
    {assign_op, '='},
    {null_op, '\0'}
}; 

const Function_info funct_info[] =
{
    {sin_f, "sin"},
    {cos_f, "cos"},
    {ln_f, "ln"},
    {tg_f, "tg"},
    {ctg_f, "ctg"},
    {sh_f, "sh"},
    {ch_f, "ch"},
    {th_f, "th"},
    {cth_f, "cth"},
    {exp_f, "exp"},
    {null_f, ""},
}; 


#endif