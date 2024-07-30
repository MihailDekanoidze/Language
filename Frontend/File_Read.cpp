#include "../include/CommonIncludes.h"
#include "../include/InputText.h"
#include "../include/Read.h"
#include "stdio.h"
#include "stdlib.h"


Program_Code* Code_Struct_Ctor(void)
{
    Program_Code* code_struct = (Program_Code*)calloc(1, sizeof(Program_Code));
    text_info* code_info = text_info_ctor();
    code_struct->code = code_info;
    code_struct->error = NO_ERROR;

    return code_struct;
}

void Code_Struct_Dtor(Program_Code* code_struct)
{
    text_info_dtor(code_struct->code);
    code_struct->error = NO_ERROR;
    free(code_struct);
}

Errors File_Read(const char* input_file, Program_Code* code_struct)
{
    FILE* code_file = FOPEN(input_file, "r");
    if (!code_file) 
    {
        LOCATION
        code_struct->error = OPEN_FILE;
        return OPEN_FILE;
    }

    code_struct->error = input_text(code_file, sizeof(char), code_struct->code);
    FCLOSE(code_file);

    return code_struct->error;
}
