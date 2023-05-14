#include "JIT.h"

#define DUMP_LIST_FILE_NAME "dump/list_dump1.gv"

#define dump(str...)                                                 \
    fprintf(dump_file, str);                                               \
    fflush(dump_file)


char* Get_Command_Name(byte_code command_type);
FILE* Open_Dump(const char* name);
void List_Dump(Byte_Code_Nodes_List* list);