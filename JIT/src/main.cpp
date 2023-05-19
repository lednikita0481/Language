#include "include/JIT.h"
#include "include/File_work.h"
#include "include/Binary_Parser.h"
#include "include/dump.h"
#include "include/Translation.h"

static const int NATIVE_MEMORY_SIZE = 1000;
static const int CODE_DATA_SPREAD = 500;

static const int PAGE_SIZE = 4096;

static const int ACCURACY = 1000;

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

    case MPROTECT_ERROR:
        printf("Memory permission change error\n");
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
    Byte_Code_Nodes_List* list = Parsing_from_binary(&text);
    List_Dump(list);
    x86_Nodes_List* x86_list = Translate(list);
    x86_List_Dump(x86_list);
    JIT_load(x86_list);
}

void In_Func(long* num)
{
    double i = 0;
    printf("Enter the number: ");
    scanf("%lf", &i);
    i = i * ((double)ACCURACY);
    long asd = (long) i;
    *num = asd;
}

void Out_Func(long int num)
{
    double i = ((double) num) / ((double) ACCURACY);
    printf("%lf\n", i);
}

void Set_Long_Number(x86_Nodes_List* x86_list, unsigned char* buffer, int* ip, u_int64_t number)
{
    *((u_int64_t*) (buffer + (*(ip)))) = number;
    *ip += sizeof(u_int64_t);
}

void Set_Int_Number(x86_Nodes_List* x86_list, unsigned char* buffer, int* ip, u_int32_t number)
{
    *((u_int32_t*) (buffer + (*(ip)))) = number;
    *ip += sizeof(u_int32_t);
}

void JIT_load(x86_Nodes_List* x86_list)
{
    unsigned char* buffer = (unsigned char*) aligned_alloc(PAGE_SIZE, PAGE_SIZE);   // suppose that 1 page is enough for our code
    int ip = 0;

    x86_list->current_using = x86_list->first_node;
    *((u_int64_t*) (buffer + ip)) = x86_list->current_using->command;
    ip += x86_list->current_using->command_size;
    Set_Long_Number(x86_list, buffer, &ip, (u_int64_t) buffer);
    x86_list->current_using = x86_list->current_using->next_node;

    for (int i = 1; i < x86_list->size; i++)
    {
        *((u_int64_t*) (buffer + ip)) = x86_list->current_using->command;
        ip += x86_list->current_using->command_size;
        
        if (x86_list->current_using->has_imm_arg)
        {
            if (x86_list->current_using->is_in_call && x86_list->current_using->command_without_mask == x86_CALL)
            {
                u_int32_t ptr = (u_int64_t) In_Func - (u_int64_t) (buffer + ip);
                Set_Int_Number(x86_list, buffer, &ip, ptr);
            }

            else if (x86_list->current_using->is_out_call)
            {
                u_int32_t ptr = (u_int64_t) Out_Func - (u_int64_t) (buffer + ip);
                Set_Int_Number(x86_list, buffer, &ip, ptr);
            }

            else if (x86_list->current_using->command_without_mask == MOV_REG_IMM || x86_list->current_using->command_without_mask == MOV_R15_IMM)
            {
                Set_Long_Number(x86_list, buffer, &ip, (u_int64_t) x86_list->current_using->imm_arg);
            }

            else 
            {
                Set_Int_Number(x86_list, buffer, &ip, x86_list->current_using->imm_arg);
            }
        }

        x86_list->current_using = x86_list->current_using->next_node;
    } 

    int mprotec_works = mprotect(buffer, PAGE_SIZE, PROT_EXEC | PROT_READ | PROT_WRITE);
    if (mprotec_works == -1) Error_Occures(MPROTECT_ERROR);

    FILE* file = fopen("debug.txt", "w");
    fwrite(buffer, sizeof(char), PAGE_SIZE, file);
    fclose(file);

    void (* run_code_god_save_me_i_have_no_idea_how_to_debug_it) (void) = (void (*) (void))(buffer);
    run_code_god_save_me_i_have_no_idea_how_to_debug_it();

}