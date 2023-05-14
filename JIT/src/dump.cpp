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
        sprintf(string, "node_%d[label = \" comand: %s | has mem_arg: %d | has imm_arg: %d | imm_arg: %d | has reg_arg: %d | reg_arg: %d\", style = \"filled\", fillcolor = %s]\n", 
                        i, Get_Command_Name(node->command_type), node->mem_arg.has_arg,
                        node->imm_arg.has_arg, node->imm_arg.arg, node->reg_arg.has_arg, node->reg_arg.arg,
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