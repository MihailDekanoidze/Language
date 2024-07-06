#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include "./CommonIncludes.h"

typedef int elem_t;
typedef uint64_t canary;

const static  elem_t POISON                        = INT_MIN;
const static  canary CANARY                        = 0xBABABABABABABABAUL;
const static  size_t DEFAULT_CAPACITY              = 25UL;
const static  size_t COUNT_ERRORS                  = 11;
const static  unsigned int UNBROKEN_POISON_SEGMENT = UINT_MAX;
const static  unsigned int INCREASE_COEF           = 20;
const static  unsigned int DECREASE_COEF           = 5;


#define CANARY_OFF         


#define LOG_FUNCTION_BEGIN if (st->stack_log != NULL) fprintf(st->stack_log, "Function %s in line %d in file %s begin\n", __PRETTY_FUNCTION__, __LINE__, __FILE__);
#define LOG_FUNCTION_END   if (st->stack_log != NULL) fprintf(st->stack_log, "Function %s in line %d in file %s end\n",   __PRETTY_FUNCTION__, __LINE__, __FILE__);
#define CANARY_OFF

#ifdef CANARY_CHECK_ON
    #define CANARY_PRINT(...) __VA_ARGS__
    #define CANARY_DATA_CREATE(...) __VA_ARGS__
    #define CANARY_CLEAR(...) __VA__ARGS__
    #define CANARY_CHECK(...) __VA_ARGS__
    #define CANARY_NEW_CAPACITY(...) __VA_ARGS__
    #define CANARY_NEW_DATA(...) __VA_ARGS__
#else
    #define CANARY_PRINT(...)
    #define CANARY_DATA_CREATE(...)
    #define CANARY_CLEAR(...)
    #define CANARY_CHECK(...)
    #define CANARY_NEW_CAPACITY(...)
    #define CANARY_NEW_DATA(...)
#endif



#define STACK_ERROR_CHECK(st) do                                \
                              {                                 \
                                  if (StackErrorCheck(&st))     \
                                      {                         \
                                        printf("");             \
                                        return st.error;        \
                                      }                         \
                              } while (0)
                              

#define CANARY_ERROR(Canary, code_error)                        \
                              do                                \
                              {                                 \
                                  if (Canary  != CANARY)        \
                                  {                             \
                                      st->error |= code_error;  \
                                      return st->error;         \
                                  }                             \
                              } while(0)                                     


enum StackError
{
    NO_ERRORS                          = 0u,
    OUT_OF_MEMORY_CALLOC               = 1u,
    OUT_OF_MEMORY_REALLOC              = 1u << 1,
    EMPTY_STACK                        = 1u << 2,
    STACK_CLEAR_ERROR                  = 1u << 3,
    STACK_NULL_ADDRESS                 = 1u << 4,
    STACK_DATA_IN_POISON_SEGMENT       = 1u << 5,
    STACK_STRUCT_LEFT_CANARY_DAMAGED   = 1u << 6,
    STACK_STRUCT_RIGHT_CANARY_DAMAGED  = 1u << 7,
    STACK_DATA_LEFT_CANARY_DAMAGED     = 1u << 8,
    STACK_DATA_RIGHT_CANARY_DAMAGED    = 1u << 9,
    STACK_POISON_IN_DATA_SEGMENT       = 1u << 10 
};



struct Stack  
{
#ifdef CANARY_ON
    canary canary_left;
    canary canary_right;
#endif


    elem_t* data;
    size_t size;
    size_t capacity;

    size_t error;
    FILE* stack_log;

};


StackError StackPrint(struct Stack *st);

StackError StackCtor(Stack* st, const size_t capacity);

StackError StackPush(struct Stack* st, const elem_t elem);

StackError StackPop(struct Stack* st, elem_t* value);

void StackClear(struct Stack* st);

int StackIsEmpty(const struct Stack* st);

elem_t StackTop(const struct Stack* st);

size_t StackSize(const struct Stack* st);

void StackDtor(struct Stack* st);

StackError StackChangeCapacity(Stack* st, const size_t);

int StackErrorCheck(struct Stack* st);

size_t CheckPoisonSegment(const Stack* st);

size_t CheckUnpoisonedSegment(const Stack* st);

void StackFilling(void* data, const void* poison,  const size_t capacity, const size_t elem_size);

size_t CanaryCheck(Stack* st);




#endif // STACK_H
