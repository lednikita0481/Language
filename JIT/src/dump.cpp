#include "include/dump.h"

#define DEF_CMD(name, num, arg, ...)\
    case  byte_code_##name:               \
        return #name;\
        break;    

char* Get_Command_Name(byte_code command_type)
{
    switch (command_type)
    {
        #include "include/cmd.h"
    
        default:
            break;
    }
}

FILE* Open_Dump(const char* name) 
{
    FILE* Dump = fopen(name, "w");
    if (Dump == NULL) printf("ERROR, CAN'T OPEN DUMP FILE\n");
    return Dump;
}

void List_Dump(Byte_Code_Nodes_List* list)
{
    FILE* dump_file = Open_Dump(DUMP_LIST_FILE_NAME);

    dump("digraph {\n");
    dump("rankdir=LR;\n");
    dump("node [ shape=record ];\n");

    Byte_Code_Node* node = list->first_node;

    for (int i = 0; i < list->nodes_amount; i++)
    {
        char string[250] = {};
        sprintf(string, "node_%d[label = \" comand: %s | ip: %d | has mem_arg: %d | has imm_arg: %d | imm_arg: %d | has reg_arg: %d | reg_arg: %d\", style = \"%sfilled\", fillcolor = %s]\n", 
                        i, Get_Command_Name(node->command_type), node->ip, node->mem_arg.has_arg,
                        node->imm_arg.has_arg, node->imm_arg.arg, node->reg_arg.has_arg, node->reg_arg.arg,
                        (node->processed)? "rounded, " : "",
                        (node->imm_arg.has_arg || node->mem_arg.has_arg || node->reg_arg.has_arg)? "green" : "red");
        dump(string);

        node = node->next_node;

        if (i == 0) continue;
        dump("node_%d -> node_%d;\n", i-1, i);
    }

    dump("}");
    fclose(dump_file);

    char cmd[] = "dot -Tpng dump/list_dump.gv -o dump/list_dump.png";
    system("iconv -f windows-1251 < dump/list_dump1.gv > dump/list_dump.gv");
    system("rm dump/list_dump1.gv");
    system(cmd);
}

char* Get_x86_Command_Name(u_int64_t command)
{
    if ((command & MOV_REG_IMM) == MOV_REG_IMM) return "MOV_REG_IMM";
    if ((command & MOV_REG_REG) == MOV_REG_REG) return "MOV_REG_REG";
    if ((command & MOV_REG_R15_OFFSET) == MOV_REG_R15_OFFSET) return "MOV_REG_R15_OFFSET";
    if ((command & MOV_R15_OFFSET_REG) == MOV_R15_OFFSET_REG) return "MOV_R15_OFFSET_REG";
    if ((command & PUSH_REG) == PUSH_REG) return "PUSH_REG";
    if ((command & POP_REG) == POP_REG) return "POP_REG";
    if ((command & POP_R15_OFFSET) == POP_R15_OFFSET) return "POP_R15_OFFSET";
    if ((command & ADD_REG_REG) == ADD_REG_REG) return "ADD_REG_REG";
    if ((command & SUB_REG_REG) == SUB_REG_REG) return "SUB_REG_REG";
    if ((command & IMUL_REG_REG) == IMUL_REG_REG) return "IMUL_REG_REG";
    if ((command & CQO) == CQO) return "CQO";
    if ((command & IDIV_REG) == IDIV_REG) return "IDIV_REG";
    if ((command & CVTSI2SD_XMM0_RAX) == CVTSI2SD_XMM0_RAX) return "CVTSI2SD_XMM0_RAX";
    if ((command & DIVPD_XMM0_XMM1) == DIVPD_XMM0_XMM1) return "DIVPD_XMM0_XMM1";
    if ((command & MULPD_XMM0_XMM1) == MULPD_XMM0_XMM1) return "MULPD_XMM0_XMM1";
    if ((command & CVTSI2SD_XMM1_RAX) == CVTSI2SD_XMM1_RAX) return "CVTSI2SD_XMM1_RAX";
    if ((command & x86_RET) == x86_RET) return "x86_RET";
    if ((command & x86_JMP) == x86_JMP) return "x86_JMP";
    if ((command & x86_COND_JMP) == x86_COND_JMP) return "x86_COND_JMP";
    if ((command & x86_CALL) == x86_CALL) return "x86_CALL";
    if ((command & SQRTPD_XMM0_XMM0) == SQRTPD_XMM0_XMM0) return "SQRTPD_XMM0_XMM0";
    if ((command & LEA_RDI_RSP) == LEA_RDI_RSP) return "LEA_RDI_RSP";
    if ((command & PUSH_ALL) == PUSH_ALL) return "PUSH_ALL";
    if ((command & POP_ALL) == POP_ALL) return "POP_ALL";
    if ((command & MOV_RBP_RSP) == MOV_RBP_RSP) return "MOV_RBP_RSP";
    if ((command & MOV_RSP_RBP) == MOV_RSP_RBP) return "MOV_RSP_RBP";
    if ((command & AND_RSP_FF) == AND_RSP_FF) return "AND_RSP_FF";
    if ((command & MOV_XMM_RSP) == MOV_XMM_RSP) return "MOV_XMM_RSP";
}


void x86_List_Dump(x86_Nodes_List* list)
{
    FILE* dump_file = Open_Dump(DUMP_x86_LIST_FILE_NAME);

    dump("digraph {\n");
    dump("rankdir=LR;\n");
    dump("node [ shape=record ];\n");

    list->current_using = list->first_node;

    for (int i = 0; i < list->size; i++)
    {
        char string[300] = {};
        sprintf(string, "node_%d[label = \" comand: %s | size: %d | old_ip: %d | new_ip: %d | has imm_arg: %d | imm_arg: %d | \", style = \"%sfilled\", fillcolor = %s]\n",
                i, Get_x86_Command_Name(list->current_using->command), list->current_using->command_size,
                 list->current_using->old_ip,
                list->current_using->new_ip, list->current_using->has_imm_arg, list->current_using->imm_arg, 
                ((list->current_using->command & x86_JMP == x86_JMP) 
                || (list->current_using->command & x86_COND_JMP == x86_COND_JMP) 
                || (list->current_using->command & x86_CALL == x86_CALL))? "rounded, " : " ",
                (list->current_using->has_imm_arg)? "red" : "green");

        dump(string);
        list->current_using = list->current_using->next_node;

        if (i == 0) continue;
        dump("node_%d -> node_%d;\n", i-1, i);
    }

    dump("}");
    fclose(dump_file);

    char cmd[] = "dot -Tpng dump/x86_list_dump.gv -o dump/x86_list_dump.png";
    system("iconv -f windows-1251 < dump/x86_list_dump1.gv > dump/x86_list_dump.gv");
    system("rm dump/x86_list_dump1.gv");
    system(cmd);
}