#include "include/JIT.h"
#include "include/File_work.h"
#include "include/Binary_Parser.h"
#include "include/dump.h"
#include "include/Translation.h"

void Error_Occures(ERRORS error, int ip, char byte)
{
    switch (error)
    {
    case FILE_INPUT_ERROR:
        printf("ERROR! Can't open input file!\n");
        break;

    case WRONG_SIGNATURE:
        printf("ERROR! Worng Signature!\n");
        break;

    case WRONG_SIZE:
        printf("ERROR! File is broken! Size is invlid\n");
        break;

    case UNKNOWN_COMMAND:
        printf("In binary file was found unknown command, ip is %d, byte %d\n", ip, byte);
        break;

    case NO_JIT_COMMAND_YET:
        printf("JIT translator doesn't support such command yet\n");
        break;

    case NO_LABEL_FOUND:
        printf("I'm an idiot and label wasn't found\n");
        break;
    
    default:
        break;
    }

    exit(error);
}

int main(int argc, char* argv[])
{
    Text text = {};
    Get_Text(&text, argc, argv);
    printf("Text readed\n");
    Byte_Code_Nodes_List* list = Parsing_from_binary(&text);
    List_Dump(list);
    x86_Nodes_List* x86_list = Translate(list);
    x86_List_Dump(x86_list);
}