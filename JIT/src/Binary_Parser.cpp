#include "include/Binary_Parser.h"

const char BYTE_CODE_MASK = (char)31;

#define DEF_CMD(name, num, arg, ...)\
    case  byte_code_##name:               \
        node->command_type = byte_code_##name;\
        break;          

Byte_Code_Nodes_List* Parsing_from_binary(Text* text)
{
    Byte_Code_Nodes_List* nodes_list = (Byte_Code_Nodes_List*) calloc(1, sizeof(Byte_Code_Nodes_List));
    nodes_list->first_node = (Byte_Code_Node*) calloc(1, sizeof(Byte_Code_Node));
    Byte_Code_Node* node = nodes_list->first_node;

    text->ip = 0;

    while (text->ip < text->size)
    {
        printf("now byte is %d, with mask: %d \n", *(text->buffer + text->ip), BYTE_CODE_MASK & *(text->buffer + text->ip));
        switch (BYTE_CODE_MASK & *(text->buffer + text->ip))
        {
            #include "include/cmd.h"

            default:
                Error_Occures(UNKNOWN_COMMAND, text->ip, *(text->buffer + text->ip));
                break;
        }
        node->ip = text->ip;

        Get_Args(node, text);
        
        node->next_node = (Byte_Code_Node*) calloc(1, sizeof(Byte_Code_Node));
        node->next_node->prev_node = node;
        nodes_list->nodes_amount++;
        node = node->next_node;
    }

    node->prev_node->next_node = NULL;
    free(node);

    node = nodes_list->first_node;
    for (int i = 0; i < nodes_list->nodes_amount; i++)
    {
        printf("%d %d\n", i, node->command_type);
        node = node->next_node;
    }

    return nodes_list;    
}


// Also increases ip
void Get_Args(Byte_Code_Node* node, Text* text)
{
    int current_ip = sizeof(char);

    if ((*(text->buffer + text->ip) & (1 << memory)) != 0) 
    {
        node->mem_arg.has_arg = true;
    }

    if ((*(text->buffer + text->ip) & (1 << imm_const)) != 0)
    {
        node->imm_arg.has_arg = true;
        node->imm_arg.arg = *((int*) (text->buffer + text->ip + current_ip));
        current_ip += sizeof(int);
    }

    if ((*(text->buffer + text->ip) & (1 << reg)) != 0)
    {
        node->reg_arg.has_arg = true;
        node->reg_arg.arg = *((int*) (text->buffer + text->ip + current_ip));
        current_ip += sizeof(int);
    }

    text->ip += current_ip;
}

#undef DEF_CMD