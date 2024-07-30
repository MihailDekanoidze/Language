#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H
#include "./DSL.h"

enum Errors
{
    NO_ERROR,
    HAS_PROBLEM,
    OPEN_FILE,
    CALLOC,
    NULL_PTR,
    NO_DEFINED_FILE,
    CAN_NOT_READ_FILE,
    UNKNOWN_FUNC,
    EMPTY_ARG
};

#define DEBUG_ON

#define INT_LEN  10
#define ACCURACY 2
#define EMPTY_COMMAND 0
#define LOCATION printf("line %d in function %s\n", __LINE__, __PRETTY_FUNCTION__);
#define ERROR_PUT(condition, dest, error, ret_val)                                      \
        if (condition)                                                                  \
        {                                                                               \
            printf("error in line %d, in fuction %s\n", __LINE__, __PRETTY_FUNCTION__); \
            dest = error;                                                               \
            return ret_val;                                                             \
        }while(0)                                           


#define ERROR_CHECK(error, actions, message)    \
if (error)                                      \
{                                               \
    printf("In line %d in funct %s\n",          \
    __LINE__, __PRETTY_FUNCTION__);             \
    printf message ;                            \
    Errors curr_error =  error;                 \
    printf("%d\n", error);                      \
    actions;                                    \
    return curr_error;                          \
} while(0)

#ifdef DEBUG_ON
#define FOPEN(file_name, mode) fopen(file_name, mode);                                              \
        fprintf(stderr, "open: %s on line: %d %s\n", #file_name, __LINE__, __PRETTY_FUNCTION__);
#else
#define FOPEN(file_name, mode) fopen(file_name, mode); 
#endif

#ifdef DEBUG_ON
#define FCLOSE(file_name) int file_close = fclose(file_name);                                       \
        fprintf(stderr, "close: %s  on line: %d %s\n", #file_name, __LINE__, __PRETTY_FUNCTION__);  \
                if (file_close != 0) {fprintf(stderr, "meow!!\n");}
#else
#define FCLOSE(file_name) int file_close = fclose(file_name);       
#endif

#ifdef DEBUG_PRINTF
#define printf(...)                                         \
        fprintf(stdout, __VA_ARGS__);                       \
        fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
#endif 



#endif //COMMON_INCLUDES_H


