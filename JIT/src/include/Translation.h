#ifndef INCLUDED_T_H
#define INCLUDED_T_H

#include "JIT.h"

struct label
{
    int old_ip = -1;
    int new_ip = 0;
};

x86_Nodes_List* Translate(Byte_Code_Nodes_List* byte_list);
void Push_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size);
void Pop_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size);
void Add_Sub_Imul_Idiv_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size);
void sqrt_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size);
void Jmp_Call_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, label* labels, int* code_size);
void CondJmp_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, label* labels, int* code_size);
void Ret_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size);
void In_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size);
void Out_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size);
void Update_Vars_And_Labels(x86_Nodes_List* x86_list, label* labels);


#define SET_COMAND(comand, ...)\
    x86_list->current_using->command = comand __VA_ARGS__;\
    x86_list->current_using->command_size = SIZE_##comand;\
    x86_list->current_using->old_ip = -1;\
    x86_list->current_using->command_without_mask = comand;\
    *(code_size) += SIZE_##comand;

#define SET_OLD_IP() x86_list->current_using->old_ip = byte_node->ip;

#define SET_IMM_ARG_MEM()\
    x86_list->current_using->has_imm_arg = true;        \
                                                        \
    if (variables[byte_node->imm_arg.arg] == 0)         \
    {                                                   \
        variables[byte_node->imm_arg.arg] = current_var;\
        current_var += X86_MEMORY_CELL;                 \
    }                                                   \
                                                        \
    x86_list->current_using->imm_arg = variables[byte_node->imm_arg.arg];\
    *(code_size) += sizeof(int);

#define SET_IMM_ARG(arg)\
    x86_list->current_using->has_imm_arg = true;        \
    x86_list->current_using->imm_arg = arg;             \
    if ((x86_list->current_using->command & MOV_REG_IMM) == MOV_REG_IMM || x86_list->current_using->command == MOV_R15_IMM) *(code_size) += sizeof(long int);\
    else *(code_size) += sizeof(int);


#define NEXT_NODE()\
    x86_list->current_using->next_node = (x86_Node*) calloc(1, sizeof(x86_Node));\
    x86_list->current_using->next_node->prev_node = x86_list->current_using;\
    x86_list->size++;\
    x86_list->current_using = x86_list->current_using->next_node;

#endif