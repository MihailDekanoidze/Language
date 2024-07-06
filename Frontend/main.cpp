#include "../include/CommonIncludes.h"
#include "../include/Read.h"
#include "../include/Token.h"
#include "../include/Differentiator.h"


int main(int argc, char** argv)
{
    Errors error  = NO_ERROR;

    if (!argc)  error = NO_DEFINED_FILE;
    ERROR_CHECK(error, ,  ("Files was not defined: error code "));

    Program_Code* input_code = Code_Struct_Ctor();
    ERROR_CHECK(input_code->error, return input_code->error, ("Can not create a Program_Code struct: error code "));

    error = File_Read(argv[1], input_code);
    ERROR_CHECK(error, , ("Can not process the input file: error code "));

    text_info* token_array = expression_tokenize(input_code->code);

    Tree* program_tree = tree_create();
    program_tree->root = get_G((Token*)(token_array->buffer));

    error = graph_image(program_tree->root, "../images/program_tree.dot");
    ERROR_CHECK(error, PROGRAMM_FINISH, ("Can not create a tree discription: error code "));

    if(system("dot ../images/program_tree.dot -T png -o ../images/program_tree.png")) return 1;
    
    printf("Programm finished without errors\n");
    PROGRAMM_FINISH;
}