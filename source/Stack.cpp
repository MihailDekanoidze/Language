#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "../include/Stack.h"

const char* stack_errors_message[] = 
{
"No errors\n",
"Out of memory for calloc\n",
"Out of memory for realloc\n",
"The stack is empty\n",
"An error occured while clearing the stack",
"Attempt to go to null adress\n",
"Data was located in poisoned segment\n",
"Damaged struct left canary\n",
"Damaged struct right canary\n",
"Damaged data left canary\n",
"Damaged data right canary\n",
"Poison in data\n"
};

StackError StackPrint(struct Stack *st)
{
    assert(st != NULL);

    LOG_FUNCTION_BEGIN

    FILE* log = st->stack_log;

    if (st == NULL)
    {
        st->error |= STACK_NULL_ADDRESS;
        return STACK_NULL_ADDRESS;
    }

    fprintf(log, "\n\nInformation of stack\n");                      
    fprintf(log, "Capacity of stack = %zu\n", st->capacity);          
    fprintf(log, "Size of stack = %zu\n", st->size);
    fprintf(log, "Address of struct stack = %p\n", st);
    fprintf(log, "Address of stack data = %p\n", st->data);
    fprintf(log, "Stack errors = %zu\n", st->error);

    if (st->size == 0) fprintf(log, "Empty stack\n");

#ifdef CANARY_ON
    fprintf(log, "\nLEFT_CANARY  = ");                                                         
    for (size_t i = 0; i < sizeof(canary); i++)
    {   
        char byte = ((char*) st->data - sizeof(canary))[i];
        fprintf(log, " %x ", byte & 0xff);
    }
    fprintf(log, "\n");
    fprintf(log, "LEFT_CANARY = %lu\n", (canary*)((char*)(st->data) - sizeof(canary)));
    fprintf(log, "RIGHT_CANARY = %lu\n", (canary*)(st->data + st->capacity));
    fprintf(log, "stack has %d byte(s) size\n", (st->capacity * sizeof(elem_t)) + 2 * sizeof(CANARY));

    for (size_t i = 0; i < (st->capacity * sizeof(elem_t)) + 2 * sizeof(CANARY); i++)
    {
        char byte = (((char*)(st->data)) - sizeof(CANARY))[i];
        
        fprintf(log, "byte[%zu] = %x\n", i, byte & 0xff);
    }
    fprintf(log, "capacity + 2 * sizeof(CANARY) / sizeof(elem_t) = %zu\n", (st->capacity * sizeof(elem_t)) + 2 * sizeof(CANARY));
#endif

    fprintf(log, "\n\n<<< SIZE >>>\n\n");


    for (size_t i = 0; i < st->size; i++)               
    {
        fprintf(log, "st[%zu] = %d\n", i, st->data[i]);
    }

    fprintf(log, "\n\n<<<FULL CAPACITY>>>\n\n");

    for (size_t i = 0; i < st->capacity; i++)               
    {
        if (st->data[i] == POISON) fprintf(log, "\tstack[%zu] = POISON\n", i);
        else {fprintf(log, "\tstack[%zu] = %d\n", i, st->data[i]);}
    }

    fprintf(st->stack_log, "\n\n");

    LOG_FUNCTION_END

    return NO_ERRORS;
}

StackError StackCtor(Stack* st, const size_t capacity)                               
{
    LOG_FUNCTION_BEGIN

    st->capacity = capacity;
    st->size = 0;

    FILE* log = fopen("stack_log.txt", "w");

    st->stack_log = log;

#ifdef CANARY_ON
    
    st->canary_left = CANARY;
    st->canary_right = CANARY;

    char* create_address = (char*)calloc(capacity * sizeof(elem_t)+ 2 * sizeof(CANARY), sizeof(char));

    *((unsigned long*)create_address) = CANARY;
    *((unsigned long*)(create_address + capacity * sizeof(elem_t) + sizeof(CANARY))) = CANARY;

    st->data = (elem_t*) (create_address + sizeof(CANARY));

    if (!create_address)
    {
        st->error = OUT_OF_MEMORY_CALLOC;
        return OUT_OF_MEMORY_CALLOC;
    }
#else // CANARY_OFF
    char* create_address = (char*) calloc(capacity * sizeof(elem_t), sizeof(char)); 

    if (!create_address)
    {
        st->error = OUT_OF_MEMORY_CALLOC;
        return OUT_OF_MEMORY_CALLOC;
    }
    
    st->data = (elem_t*) (create_address);
#endif

    StackFilling(st->data, &(POISON), capacity, sizeof(elem_t));

    LOG_FUNCTION_END

    return NO_ERRORS;
}

StackError StackPush(struct Stack* st, const elem_t elem)
{
    LOG_FUNCTION_BEGIN

    fprintf(st->stack_log, "Command_PUSH (%d)\n", elem);

    if (st->capacity == st->size)
    {
        if (StackChangeCapacity(st, INCREASE_COEF)) return OUT_OF_MEMORY_REALLOC;
    }

    st->data[st->size++] = elem;

    LOG_FUNCTION_END

    return NO_ERRORS;
}

StackError StackPop(struct Stack* st, elem_t* value)
{
    LOG_FUNCTION_BEGIN

    if (st->size == 0)
    {
        st->error |= EMPTY_STACK;      
        return EMPTY_STACK;
    }

    if (value != NULL)
    {
        *value = st->data[--st->size];
        st->data[st->size] = POISON;

        if (st->size < st->capacity / 2 && st->size > DEFAULT_CAPACITY)                                                            
        {
            if (StackChangeCapacity(st, DECREASE_COEF)) 
            {
                st->error |= OUT_OF_MEMORY_REALLOC;
                return OUT_OF_MEMORY_REALLOC;
            }
        }

        fprintf(st->stack_log, "Command_POP(%d)\n", *value);
    }

    LOG_FUNCTION_END

    return NO_ERRORS;

}

void StackDtor(struct Stack* st)
{   
    

    LOG_FUNCTION_BEGIN

    StackClear(st);

#ifdef CANARY_ON
    free((char*)(st->data) - sizeof(CANARY));
#else //CANARY_OFF
    free((char*)(st->data));
#endif

    st->data = NULL;

    st->capacity = 0;
    st->error |= NO_ERRORS;
    st->size = 0;

    fclose(st->stack_log);

    //printf("Meow!\n");


    LOG_FUNCTION_END
}

size_t StackSize(const struct Stack* st)
{
    LOG_FUNCTION_BEGIN

    return st->size;

    LOG_FUNCTION_END
}

elem_t StackTop(const struct Stack* st)     
{
    LOG_FUNCTION_BEGIN

    if (!StackIsEmpty(st))
    {
        fprintf(st->stack_log, "Command_TOP(%d)\n", st->data[st->size-1]);    
        return st->data[st->size - 1];
    }
    else
    {
        fprintf(st->stack_log, "Command_TOP(POISON)\n");    
        return POISON;
    }

    LOG_FUNCTION_END           
}

int StackIsEmpty(const struct Stack* st)
{
    LOG_FUNCTION_BEGIN
 
    return (st->size == 0);

    LOG_FUNCTION_END
}

void StackClear(struct Stack* st)     
{
    LOG_FUNCTION_BEGIN

    StackFilling(st->data, &POISON, st->capacity, sizeof(elem_t));

    st->size = 0; 

    LOG_FUNCTION_END
}

int StackErrorCheck(struct Stack* st)
{
    LOG_FUNCTION_BEGIN

    fprintf(st->stack_log, "Stack errors = %zu\n", st->error);

    size_t no_poisoned = CheckPoisonSegment(st);

    if (no_poisoned != UNBROKEN_POISON_SEGMENT)
    {
        st->error |= STACK_DATA_IN_POISON_SEGMENT;
        fprintf(st->stack_log, "no_poisoned[%zu] = %d\n", no_poisoned, st->data[no_poisoned]);
    }

    size_t poisoned_elem = CheckUnpoisonedSegment(st);

    if (poisoned_elem != st->size)
    {
        st->error |= STACK_POISON_IN_DATA_SEGMENT;
        fprintf(st->stack_log, "poisoned[%zu] = %d\n", poisoned_elem, st->data[poisoned_elem]);
    }

#ifdef CANARY_ON
    CanaryCheck(st);
#endif

    fprintf(st->stack_log, "Error = %zu \n", st->error);
    
    if (st->error == NO_ERRORS) {return NO_ERRORS; LOG_FUNCTION_END}

    for (size_t i = 0; i < COUNT_ERRORS; i++)         
    {
        if (st->error & (1 << i)) fprintf(st->stack_log, "%s", stack_errors_message[i + 1]);    
    }

    
    LOG_FUNCTION_END
    
    return (int)st->error;
}

StackError StackChangeCapacity(Stack* st, const size_t coef_capacity)
{
    LOG_FUNCTION_BEGIN

#ifdef CANARY_ON
    size_t new_capacity = (st->capacity * coef_capacity * sizeof(elem_t) / 10UL + 2 * sizeof(CANARY));
    fprintf(st->stack_log, "new_capacity = %zu(bytes)\n", new_capacity);
    char* data_address = (char*) realloc((char*)(st->data) - sizeof(CANARY), new_capacity); 
#else //CANARY_OFF
    size_t new_capacity = (st->capacity * coef_capacity * sizeof(elem_t) / 10UL);    
    char* data_address = (char*) realloc((char*)(st->data), new_capacity);
#endif

    if (data_address == NULL)
    {
        st->error |= OUT_OF_MEMORY_REALLOC;
        return OUT_OF_MEMORY_REALLOC;
    }

    st->capacity = st->capacity * coef_capacity / 10UL;

    fprintf(st->stack_log,"capacity =%zu\n", st->capacity);

#ifdef CANARY_ON
    *((unsigned long long*)data_address) = CANARY;
    *((unsigned long long*)(data_address + sizeof(CANARY) + st->capacity * sizeof(elem_t))) = CANARY;
    st->data = (elem_t*)(data_address + sizeof(CANARY));
#else //CANARY_OFF
    st->data = (elem_t*)(data_address);
#endif

    StackFilling(st->data + st->size, (const void*)&POISON, st->capacity - st->size, sizeof(elem_t));

    LOG_FUNCTION_END
    
    return NO_ERRORS;
}


size_t CheckPoisonSegment(const Stack* st)
{
    LOG_FUNCTION_BEGIN    

    size_t no_poisoned_elem = UNBROKEN_POISON_SEGMENT;

    
    for (size_t i = st->size; i < st->capacity; i++)
    {
        if (st->data[i] != POISON)
        {
            no_poisoned_elem = i;
            break;
        }
    }

    LOG_FUNCTION_END

    return no_poisoned_elem;
}


void StackFilling(void* data, const void* poison, const size_t elem_count, const size_t elem_size)   
{
    for (size_t i = 0; i < elem_count; i++)
    {
        memcpy((char*)data + i * elem_size, poison, elem_size); 
    }
}

size_t CheckUnpoisonedSegment(const Stack* st)
{
    LOG_FUNCTION_BEGIN

    size_t poisoned_elem = st->size;

    for (size_t i = 0; i < st->size; i++)
    {
        if (st->data[i] == POISON)
        {
            poisoned_elem = i;
            break;
        }
    }

    LOG_FUNCTION_END

    return poisoned_elem;
}

       

size_t CanaryCheck(Stack* st)
{
    LOG_FUNCTION_BEGIN

#ifdef CANARY_ON
    CANARY_ERROR(st->canary_left,  STACK_STRUCT_LEFT_CANARY_DAMAGED);
    CANARY_ERROR(st->canary_right, STACK_STRUCT_RIGHT_CANARY_DAMAGED);
#endif

    char* left_canary  = (char*)(st->data) - sizeof(CANARY);
    char* right_canary = (char*)(st->data) + sizeof(elem_t) * st->capacity;

    fprintf(st->stack_log, "left_canary = %lu\n",   *((unsigned long*) left_canary  ));
    fprintf(st->stack_log, "right_canary  = %lu\n", *((unsigned long*) right_canary ));
    

    CANARY_ERROR(*((unsigned long*) left_canary),   STACK_DATA_LEFT_CANARY_DAMAGED);
    CANARY_ERROR(*((unsigned long*) right_canary),  STACK_DATA_LEFT_CANARY_DAMAGED); 

    LOG_FUNCTION_END

    return NO_ERRORS;
}