#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/Tree.h"
#include "../include/Stack.h"
#include "../include/ExtraFunctions.h"
#include "../include/Read.h"

Tree* tree_create(void)
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    assert(tree);

    tree->root = NULL;

    /*tree->root->left = NULL;
    tree->root->right = NULL;
    tree->root->previous = NULL;*/

    FILE* tree_log = fopen("../logs/tree_log.txt", "w");
    tree->tree_log = tree_log;

    assert(tree);
    
    return tree;
}

void tree_detor(Tree* tree)
{
    if (!tree) return;
    node_dtor_all(tree->root);
    
    if (tree->tree_log) fclose(tree->tree_log);

    free(tree);
}

void node_print(const Node* N, FILE* tree_data)
{
    if(!N) return;

    if((L != NULL) && (R != NULL)) fprintf(tree_data, "(");
    if ((R == NULL) && (L == NULL))
    {
        switch(ND)
        {
        case number:
        {
            if (NVN < 0) 
            {
                fprintf(tree_data, "(%lg)", NVN);
                break;
            }

            fprintf(tree_data, "%lg", NVN);
            break;
        }
        case var:
        {
            fprintf(tree_data, "%s", (NVV));
            break;
        }
        case empty_node:
            printf("Empty node\n");
        case func:
        case operation:
        case key_word:
        default:
            break;
        }
    }
    else
    {
        //printf("\nleft print\n");
        node_print(L, tree_data);

        switch (ND)
        {
        case key_word:
            fprintf(tree_data, "%s", get_key_word_str(NVKW));
            break;
        case operation:
            fprintf(tree_data, "%c", get_oper_symbol(NVO));
            break;
        case func:
            fprint_func(tree_data, NVF);
        case var:
        case number:
        case empty_node:
        default:
            break;
        }
    
        //printf("\nright print\n");
        node_print(R, tree_data);
    }

    if((L != NULL) && (R != NULL)) fprintf(tree_data, ")");
}

void print_curr_node(const Node* N)
{
    switch (ND)
    {                                       
    case number:
        printf(" #%lg# ", NVN);
        break;
    case var:
        printf(" #%s# ", NVV);
        break;
    case operation:
        printf(" %c ", get_oper_symbol(NVO));
        break;
    case func:
        fprint_func(stdout, NVF);
        break;
    case key_word:
        printf(" %s ", get_key_word_str(NVKW));
        break;
    case empty_node:
        printf("Empty token\n");
        break;
    default:
        printf("Unknown node type <%lln>\n", (long long*)(NV));
        break;
    }
}


void skip_spaces(char** source)
{
    while (isspace(**(source)) || (**(source) == '\n'))
    {
        (*source)++;
    }
}

void skip_alpha(char* source, size_t* pos)
{
    printf("cur symbol is %c\n", *(source + *pos));
    while (isalpha(*(source + *pos)))
    {
        printf("alpfa symbol is %c\n", *(source + *pos));
        (*pos)++;
    }
    printf("cur symbol is %c\n", *(source + *pos));
}


void ClearBuffer(void)
{
    char ch = 0;

    while ((ch = (char)getchar()) != '\n' && ch != EOF) {}
}

void fprint_nchar(FILE* dest, char symbol, size_t count)
{
    for (size_t i = 0; i < count; i++) {fprintf(dest, "%c", symbol);}
}

Node* tree_add_node(Node* parent, Child subtree, Type tp, void* arg)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    
    new_node->left  = NULL;
    new_node->right = NULL;
    new_node->previous = parent;
    new_node->val = (node_data*) calloc(1, sizeof(node_data));
    
    if (tp == operation)
    {
        new_node->data_type = operation;
        new_node->val->op = *((Operation*)arg);
    }
    else
    {
        new_node->data_type = number;
        new_node->val->number = *((double*)arg);
    }

    switch (subtree)
    {
        case left:
        {
            parent->left = new_node;
            break;
        }
        case right:
        {
            parent->right = new_node;
            break;
        }
        default:
        {
            printf("Неправильный ребёнок\n");
            return NULL;
        }
    }

    return new_node;
}



void node_dtor_all(Node* N)
{
    if (N == NULL) return;

    if (ND == var) free (NV->var);
    free(NV);

    node_dtor_all(L);
    node_dtor_all(R);

    free(N);

    return;
}

void node_dtor_one(Node* N, Child saved_child)
{
    if (N == NULL) return;
    free(NV);

    Node* saved_node = NULL;
    if (saved_child == left)
    {
        saved_node = L;
        free(NRV);
        free(R);
    }
    else    
    {
        saved_node = R;
        free(NLV);
        free(L);
    }

    ND = saved_node->data_type;
    L = saved_node->left;
    R = saved_node->right;    
    NV = saved_node->val;

    free(saved_node);

    return;
}

Operation get_oper_code(const char* source)
{
    for (size_t i = 0; i < OPERATION_COUNT; i++)
        if (*source == op_info[i].symbol) return op_info[i].op;

    //printf("Expected unknown oper\n");
    return null_op;
}
Function get_funct_code(const char* source)
{
    for (size_t i = 0; i < FUNCTION_COUNT; i++)
    {
        if (!strncmp(funct_info[i].name, source, strlen(funct_info[i].name))) return funct_info[i].function;
    }
        
    //printf("Expected unknown func\n");
    return null_f;
}

char get_oper_symbol(Operation op)
{
    for (size_t i = 0; i < OPERATION_COUNT; i++)
        if (op == op_info[i].op) return op_info[i].symbol;
    
    //printf("Expected unknown oper\n");
    return 0;
}

const char* get_key_word_str(Key_word curr_word)
{
    for (size_t i = 0; i < KEY_WORD_COUNT; i++)
        if (curr_word == key_word_info[i].word) return key_word_info[i].name;

    return 0;
}

Key_word get_key_word_number(const char* data)
{
    for (size_t i = 0; i < KEY_WORD_COUNT; i++)
    {

        #ifdef DEBUG_ON
        printf("i = %zu\n", i);
        printf("Original key_word len is %zu\n", strlen(key_word_info[i].name));
        printf("Compare keyword between %s and ", key_word_info[i].name);
        for (size_t j = 0; j < strlen(key_word_info[i].name); j++)
        {
            printf("%c", *(data + j));
        }
        printf("\n");
        #endif

        if (!strncmp(data, key_word_info[i].name, strlen(key_word_info[i].name)))
            return key_word_info[i].word;
    }
    return null_w;
}

void fprint_arg(FILE* dest, const Node* N)
{
    switch (ND)
    {                                       
    case number:
        fprintf(dest, " %lg ", NVN);
        break;
    case var:
        fprintf(dest, " %s ", NVV);
        break;
    case operation:
        fprintf(dest, " %c ", get_oper_symbol(NVO));
        break;
    case func:
        fprint_func(dest, NVF);
        break;
    case key_word:
        fprintf(dest, " %s ", get_key_word_str(NVKW));
        break;
    case empty_node:
        fprintf(dest, "Empty node\n");
        break;
    default:
        fprintf(dest, "Unknown type <%lln>\n", (long long*)(NV));
        break;
    }
}

void fprint_func(FILE* dest, Function func)
{
    //printf("print_funct get %d\n", func);
    switch (func)
    {
    case sin_f:
        fprintf(dest, "sin");
        break;
    case cos_f:
        fprintf(dest, "cos");
        break;
    case ln_f:
        fprintf(dest, "ln");
        break;
    case tg_f:
        fprintf(dest, "tg");
        break;
    case ctg_f:
        fprintf(dest, "ctg");
        break;
    case sh_f:
        fprintf(dest, "sh");
        break;
    case ch_f:
        fprintf(dest, "ch");
        break;
    case th_f:
        fprintf(dest, "th");
        break;
    case cth_f:
        fprintf(dest, "cth");
        break;
    case exp_f:
        fprintf(dest, "exp");
        break;
    case null_f:
    default:
        printf("Unknown function\n");
        break;
    }
}

const char* get_funct_name(Function funct)
{
    for (size_t i = 0; i < FUNCTION_COUNT; i++)
        if (funct == funct_info[i].function) return funct_info[i].name;
    
    //printf("Expected unknown func\n");
    return 0;
}


Errors graph_image(const Node* start, const char* name)
{
    FILE* tree_info = fopen(name, "w");

    if (!tree_info)
    {
        printf("tree.dot has null ptr!\n");
        return OPEN_FILE;
    }

    fprintf(tree_info, "digraph G\n");
    fprintf(tree_info, "{\n");

    fprintf(tree_info, "rankdir=TB;\n");
    fprintf(tree_info, "node[color=\"red\"shape=\"rectangle\",style=\"rounded\"];\n");

    START_NODE_CHILD_PRINT(tree_info, start, start->left);
    START_NODE_CHILD_PRINT(tree_info, start, start->right);

    fprintf(tree_info, "}%c", '\0');

    fclose(tree_info);

    return NO_ERROR;
}

void node_dot_create(const Node* N, FILE* tree_info)
{
    if (!N) return;
    
    #ifdef DEBUG_ON            
    printf("in line %d %s\n", __LINE__, __PRETTY_FUNCTION__);
    print_curr_node(N);
    printf("\n%p\n", N);
    #endif


    if ((L == NULL) && (R == NULL))
    {
        fprintf(tree_info, "->\"");                        
        fprint_arg(tree_info, N); 
        fprintf(tree_info, "\n\n%p", N);                    
        fprintf(tree_info, "\";\n");
        return;
    }

    NODE_CHILD_PRINT(tree_info, N, L);
    START_NODE_CHILD_PRINT(tree_info, N, R);

    return;
}

node_data* val_double(double data)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    val->number = data;   
    return val;                                                         
}

node_data* val_Key_word(Key_word key_word)
{
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));
    val->key_word = key_word;
    return val;
}

node_data* val_Operation(Operation op)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    val->op = op;
    return val;                                                         
}

node_data* val_Function(Function func)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    val->func = func;
    return val;                                                         
}

node_data* val_var(const char* var)         
{              
    if (!var) 
    {
        printf("var is null\n");
        return NULL;
    }

    union node_data* val = (node_data*)calloc(1, sizeof(node_data)); 
    val->var = (char*)calloc(strlen(var) + 1, sizeof(char));   
    memcpy(val->var, var, strlen(var) + 1);

    if(!val) 
    {
        printf("in val_var val is null\n");
        return NULL;
    }
    else if (!(val->var)) 
    {
        printf("in val_var val->var is null\n");
        return NULL;
    }
    return val;                                                         
}

Node* copy_node(const Node* N)
{
    if (N == NULL) return NULL;

    Node* copied_node = (Node*)calloc(1, sizeof(Node));
    copied_node->val = (node_data*)calloc(1, sizeof(node_data));

    memcpy(&copied_node->data_type, &ND, sizeof(Type));
    if (ND != var) memcpy(copied_node->val, NV, sizeof(node_data));
    else
    {
        size_t len = strlen(NVV) + 1;
        copied_node->val->var = (char*)calloc(len, sizeof(char));   
        memcpy(copied_node->val->var, NVV, len);
    }

    copied_node->left  = copy_node(L);
    copied_node->right = copy_node(R);

    return copied_node;
}

Node* create_node(Type data_type, node_data* val, Node* left, Node* right)
{
    if (!val) {printf("node_data has null ptr\n");}
    Node* N = (Node*) calloc(1, sizeof(Node));
    NV = val;
    ND = data_type;

    L  = left;
    R = right;

    return N;   
}



