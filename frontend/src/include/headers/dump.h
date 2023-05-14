#include "Language.h"
#include "DSL.h"
#include <string.h>

#define DUMP_TOKEN_FILE_NAME "dump/token1.gv"
#define DUMP_PROGA_FILE_NAME "dump/proga1.gv"
const char png[5] = ".png";


#define Dump(str...)                                                 \
    fprintf(Dump_File, str);                                               \
    fflush(Dump_File)

FILE* Open_Dump(const char* name);
void Token_Dump(Node** nodes, int amount);
const char* Spec_Type_Name(Language_Types type);
const char* Spec_Type_Name_Standart(Language_Types type);
void Proga_Dump(Node* Main_Node);
void Recursive_Node_Dump(const Node* node, FILE* Dump_File);
void Standart_Dump(const int argc, const char* argv[], Node* Main_Node);
void Standart_Recursion(FILE* Dump_File, int tabs, Node* node);