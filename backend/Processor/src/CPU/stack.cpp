#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "stack.h"

//-------------------------------------------------------------------------

int errors_blin_kill_that_shit = 0;

//-------------------------------------------------------------------------

int                ReturnPoison(int)                { return integer_poison; }
size_t             ReturnPoison(size_t)             { return integer_poison; }
double             ReturnPoison(double)              { return double_poison; }
char               ReturnPoison(char)                      { return 'z' + 1; }
char*              ReturnPoison(char*)                        { return NULL; }
unsigned long long ReturnPoison(unsigned long long) { return integer_poison; }

//-------------------------------------------------------------------------

void StackConstructor(Stack* stk, size_t capacity)
{
    stk->Stack_Left_Cannery = STACK_LEFT_CANNERY_VALUE;

    stk->capacity = capacity;

    char* buffer = (char*)calloc((capacity*sizeof(Elem_t) + 2*sizeof(CANNERY_SIZE)), sizeof(char));

      stk->Data_Left_Cannery   = (CANNERY_SIZE*)buffer;
    *(stk->Data_Left_Cannery)  = DATA_LEFT_CANNERY_VALUE;

      stk->data                = (Elem_t*)(buffer + sizeof(CANNERY_SIZE));

      stk->Data_Right_Cannery  = (CANNERY_SIZE*)((char*)stk->data + sizeof(Elem_t)*(stk->capacity));
    *(stk->Data_Right_Cannery) = DATA_RIGHT_CANNERY_VALUE;


    stk->size = 0;

    stk->Stack_Right_Cannery = STACK_RIGHT_CANNERY_VALUE;

    #if (defined(NDEBUG))
        Stack_Calculate_Hash(stk);
    #endif

    ASSERT_OK(stk);
}

void StackDestructor(Stack* stk)
{
    ASSERT_OK(stk);

    *(stk->Data_Left_Cannery) = ReturnPoison(DATA_LEFT_CANNERY_VALUE);
    *(stk->Data_Right_Cannery) = ReturnPoison(DATA_RIGHT_CANNERY_VALUE);

    stk->Data_Left_Cannery = NULL;
    stk->Data_Right_Cannery = NULL;
    stk->Stack_Left_Cannery = ReturnPoison(stk->Stack_Left_Cannery);
    stk->Stack_Right_Cannery = ReturnPoison(stk->Stack_Right_Cannery);

    //memset

    /*
    for (size_t i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = ReturnPoison(stk->data[i]);
    }
    */
    memset(stk->data, ReturnPoison(stk->data[0]), stk->capacity);

    *(stk->Data_Left_Cannery) = ReturnPoison(*(stk->Data_Left_Cannery));
    *(stk->Data_Right_Cannery) = ReturnPoison(*(stk->Data_Right_Cannery));

    free(stk->data);
    free(stk->Data_Left_Cannery);
    free(stk->Data_Right_Cannery);

    stk->capacity = ReturnPoison(stk->capacity);
    stk->size = ReturnPoison(stk->size);
    stk->data = NULL;
    stk->Data_Left_Cannery = NULL;
    stk->Data_Right_Cannery = NULL;
    stk->Stack_Left_Cannery = ReturnPoison(stk->Stack_Left_Cannery);
    stk->Stack_Right_Cannery = ReturnPoison(stk->Stack_Right_Cannery);
}

void StackPush(Stack* stk, Elem_t value)
{
    ASSERT_OK(stk);

    if (stk->size >= stk->capacity)
    {
        StackRealloc(stk);
    }

    stk->data[stk->size++] = value;

    #if (defined(NDEBUG)) 
        Stack_Calculate_Hash(stk);
    #endif
    

    ASSERT_OK(stk);
}

Elem_t StackPop(Stack *stk)
{
    ASSERT_OK(stk);
    ASSERT_POP(stk);

    Elem_t element = stk->data[--stk->size];
    stk->data[stk->size] = ReturnPoison(stk->data[stk->size]);

    #if (defined(NDEBUG)) 
        Stack_Calculate_Hash(stk);
    #endif

    if ((stk->size) * 4 <= stk->capacity && stk->capacity > 5)
    {
        StackRealloc(stk);
    }

    #if (defined(NDEBUG)) 
        Stack_Calculate_Hash(stk);
    #endif

    ASSERT_OK(stk);

    return element;
}

void StackRealloc(Stack* stk)
{
    ASSERT_OK(stk);

    char* tmp = NULL;
    if (stk->size >= stk->capacity)
    {
        tmp = (char*)recalloc(stk->Data_Left_Cannery, (2*sizeof(CANNERY_SIZE) + sizeof(Elem_t)*(stk->capacity)*2),
                                         sizeof(char), (sizeof(CANNERY_SIZE) + (sizeof(Elem_t)*(stk->capacity))));
        stk->capacity *= 2;
    }

    else
    {
        tmp = (char*)recalloc(stk->Data_Left_Cannery, (2*sizeof(CANNERY_SIZE) + sizeof(Elem_t)*(stk->capacity)/2),
                                          sizeof(int), (2*sizeof(CANNERY_SIZE) + (sizeof(Elem_t)*(stk->capacity))));
        stk->capacity /= 2;
    }

    stk->Data_Left_Cannery = (CANNERY_SIZE*)tmp;
    stk->data = (Elem_t*)(tmp + sizeof(CANNERY_SIZE));
    stk->Data_Right_Cannery = (CANNERY_SIZE*)((char*)stk->data + (sizeof(Elem_t)*(stk->capacity)));
    *(stk->Data_Right_Cannery) = DATA_RIGHT_CANNERY_VALUE;

    #if (defined(NDEBUG)) 
        Stack_Calculate_Hash(stk);
    #endif

    ASSERT_OK(stk);
}

void* recalloc(void* array, size_t num, size_t elem_size, size_t elements_now)
{
    char* ptr = (char*)array;

    size_t bytes_to_realloc = num*elem_size;

    ptr = (char*)realloc(ptr, bytes_to_realloc);

    if (num > elements_now)
    {
        memset((ptr + elem_size*elements_now), '\0', (bytes_to_realloc - elem_size*elements_now));
        /*
        for (size_t reset = elem_size*elements_now; reset < bytes_to_realloc; reset++)
        {
            ptr[reset] = '\0';
        }*/
    }

    return ptr;
}

//|= 
int CheckStack(Stack* stk, bool Called_From_Pop)
{
    int err_code = No_Errors;

    if(stk == NULL)
    {
        err_code += Null_Ptr_Err;
        return err_code;
    }

    if (Called_From_Pop && stk->size <= 0)
    {
        err_code += Pop_Impossible_Err;
    }

    if (stk->capacity == 0)
    {
        err_code += Null_Capacity_Err;
    }

    if (stk->capacity < stk->size)
    {
        err_code += Size_More_Cap_Err;
    }

    if (stk->data == NULL)
    {
        err_code += Data_Null_Ptr_Err;
    }

    if (*(stk->Data_Left_Cannery) != DATA_LEFT_CANNERY_VALUE)
    {
        err_code += Data_Left_Can_Err;
    }

    if (*(stk->Data_Right_Cannery) != DATA_RIGHT_CANNERY_VALUE)
    {
        err_code += Data_Right_Can_Err;
    }

    if (stk->Stack_Left_Cannery != STACK_LEFT_CANNERY_VALUE)
    {
        err_code += St_Left_Can_Err;
    }

    if (stk->Stack_Right_Cannery != STACK_RIGHT_CANNERY_VALUE)
    {
        err_code += St_Right_Can_Err;
    }

    #if (defined(NDEBUG)) 

        size_t tmp_data_hash = stk->data_hash;
        size_t tmp_stk_hash = stk->stack_hash;
        stk->stack_hash = 0;
        Stack_Calculate_Hash(stk);
        
        if (stk->data_hash != tmp_data_hash)
        {
            err_code += Data_Hash_Err;
        }
        
        if (stk->stack_hash != tmp_stk_hash)
        {
            err_code += St_Hash_err;
        }
    
    #endif

    return err_code;
}

//!print stack errors
void PrintStackErrors(FILE* stream, int error)
{
    if (error == 0)
    {
        fprintf(stream, "Ok\n");
    }

    if ((error & Null_Ptr_Err) != 0)
    {
        fprintf(stream, "Bad pointer. Impossible to read data\n");
    }

    if ((error & Pop_Impossible_Err) != 0)
    {
        fprintf(stream, "Impossible to pop smth from stack with 0 size\n");
    }

    if ((error & Null_Capacity_Err) != 0)
    {
        fprintf(stream, "Capacity = 0, stack doesn't make sence\n");
    }

    if ((error & Size_More_Cap_Err) != 0)
    {
        fprintf(stream, "Size is bigger then capacity. Cringe\n");
    }

    if ((error & Data_Null_Ptr_Err) != 0)
    {
        fprintf(stream, "Impossible to read data\n");
    }

    if ((error & Data_Left_Can_Err) != 0)
    {
        fprintf(stream, "Left Cannery for data broken\n");
    }

    if ((error & Data_Right_Can_Err) != 0)
    {
        fprintf(stream, "Right Cannery for data broken\n");
    }

    if ((error & St_Left_Can_Err) != 0)
    {
        fprintf(stream, "Left cannery for stack broken\n");
    }

    if ((error & St_Right_Can_Err) != 0)
    {
        fprintf(stream, "Right cannery for stack broken\n");
    }

    #if (defined(NDEBUG))
        if ((error & Data_Hash_Err) != 0)\
        {\
            fprintf(stream, "!!! Hash of data broken\n");\
        }\
        \
        if ((error & St_Hash_err) != 0)\
        {\
            fprintf(stream, "!!! Hash of stack broken\n");\
        }
    #endif
}

void exit_errors(int error)
{
    if (error != 0)
    {
        printf("Stack ERROR, epta!\n");
        PrintStackErrors(stdout, error);
        exit(error);
    }
}

void StackDuump(Stack* stk, int error, const char* func, const char* file, const int line)
{
    char File_name[] = "debug.txt";
    FILE* File = fopen(File_name, "a");

    fprintf(File, LINES);
    fprintf(File, "\n");
    fprintf(File, "%s at %s(%d):\n", func, file, line);
    fprintf(File, "Stack[%p]", stk);

    if (!error)
    {
        fprintf(File, "(ok), made at %s in %s(%d) as %s\n", stk->info.function, stk->info.file, stk->info.line, stk->info.first_name);
        fprintf(File, "{\n");
        fprintf(File, "    size     = %d\n", stk->size);
        fprintf(File, "    capacity = %d\n", stk->capacity);
        fprintf(File, "    data[%p]\n", stk->data);
        fprintf(File, "    {\n");

        for (int i = 0; i < stk->capacity; i++)
        {
            fprintf(File, "        %c[%d] = %d      %s\n", ((i<stk->size)? '*' : ' '), i, stk->data[i], ((stk->data[i] == ReturnPoison(stk->data[i]))? "(POISON)" : " "));
        }

        fprintf(File, "    }\n");
        fprintf(File, "}\n");
    }

    else
    {
        fprintf(File, "(ERROR) \n");
        if ((error & Null_Ptr_Err) != 0)
        {
            fprintf(File, "Bad pointer, impossible to count data\n");
        }

        else
        {
            PrintStackErrors(File, error);
        }
    }

    fclose(File);
}

size_t Calculate_Hash(void* Structure, size_t size_bites)
{
    char* structure = (char*)Structure;
    size_t control_sum = 0xDED9EBE1;
    for (size_t i = 0; i < size_bites; i++)
    {
        control_sum += i * structure[i];
    }

    return (((control_sum << 5) + control_sum) + 1);
}

#if (defined(NDEBUG))
    void Stack_Calculate_Hash(Stack* stk) \
    {                                     \
        stk->stack_hash = 0;\
        stk->data_hash = Calculate_Hash(stk->Data_Left_Cannery, (2*sizeof(unsigned long long) + sizeof(Elem_t)*(stk->capacity)));\
        stk->stack_hash = Calculate_Hash(stk, sizeof(Stack));\
    }
#endif