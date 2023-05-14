#ifndef INCLUDED_P_H
#define INCLUDED_P_H

#include "JIT.h"

enum shift
{
    imm_const = 5,
    reg       = 6,
    memory    = 7
};

Byte_Code_Nodes_List* Parsing_from_binary(Text* text);
void Get_Args(Byte_Code_Node* node, Text* text);

#endif