#ifndef INCLUDED_J_H
#define INCLUDED_J_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DEF_CMD(name, num, arg, ...)\
        byte_code_##name = num,  

enum byte_code 
{
    #include "cmd.h"
};

#undef DEF_CMD

struct Argument
{
    bool has_arg;
    int arg;
};

struct Byte_Code_Node
{
    int ip;
    byte_code command_type;
    Argument mem_arg;
    Argument reg_arg;
    Argument imm_arg;
    Byte_Code_Node* next_node;
    Byte_Code_Node* prev_node;
};

struct Byte_Code_Nodes_List
{
    Byte_Code_Node* first_node;
    int nodes_amount;
};

struct Text
{
    char* buffer;
    size_t size;
    int ip;
};


enum ERRORS
{
    NO_ERROR,
    FILE_INPUT_ERROR,
    WRONG_SIGNATURE,
    WRONG_SIZE,
    UNKNOWN_COMMAND,
};


void Error_Occures(ERRORS error, int ip = -1, char byte = 0);

#endif