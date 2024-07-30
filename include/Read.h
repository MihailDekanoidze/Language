#ifndef READ_TOKENIZE_H
#define READ_TOKENIZE_H


#include "./InputText.h"
#include "./Token.h"


#define PROGRAMM_FINISH                                         \
        tree_detor(program_tree);                               \
        Code_Struct_Dtor(input_code);                           \
        token_array_dtor(token_array);                          

typedef struct Program_Code
{
    text_info* code;
    Errors error;
}  Program_Code;


Program_Code*           Code_Struct_Ctor(void);
void                    Code_Struct_Dtor(Program_Code* code_struct);
Errors                  File_Read(const char* input_file, Program_Code* code_struct);



#endif

