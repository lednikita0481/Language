#include <stdlib.h>

#define DEF_CMD(name, num, arg, ...)\
        cmd_##name = num,  
enum Commands
{
    #include "../cmd.h"
};
#undef DEF_CMD

#define DEF_CMD(name, num, arg, ...)                            \
        else if (strcasecmp(command, #name) == 0)                  \
        {                                                       \
            if (arg)                                            \
            {                                                   \
                char* Line = text->lines[line];                 \
                printf("Line in push pop is %s\n", Line);       \
                int Com_Name = cmd_##name;                      \
                Get_arg(Line, code, &pointer, Com_Name, line);  \
                line++;                                         \
            }                                                   \
            else                                                \
            {                                                   \
                int Com_Name = cmd_##name;                      \
                *(code + pointer) = char(Com_Name);             \
                pointer += sizeof(char);                        \
                line++;                                         \
            }                                                   \
        }

#define DEF_JMP(c_name)                                         \
    else if (strcmp(command, #c_name) == 0)                    \
    {                                                           \
        printf("Trying to add label\n");                        \
        char* Line = text->lines[line];                         \
        Reach_Space(&Line);                                     \
                                                                \
        char name[LABEL_NAME_LEN] = {};                         \
                                                                \
        if (sscanf(Line, "%s", name) == 1)                      \
        {                                                       \
            printf("label has a name\n");                       \
                                                                \
            int Com_Name = cmd_##c_name + (1 << imm_const);                        \
            *(code + pointer) = char(Com_Name);                 \
                                                                \
            pointer += sizeof(char);                            \
                                                                \
            find_label(name, labels, code, &pointer);           \
        }                                                       \
                                                                \
        else                                                    \
        {                                                       \
            Errors_Processing(JMP_ERROR, line);                 \
        }                                                       \
                                                                \
        line++;                                                 \
    }

enum shift
{
    imm_const = 5,
    reg       = 6,
    memory    = 7
};


const int LABEL_NAME_LEN = 25;
const int LABEL_POISON = -1;
struct label
{
    int line = LABEL_POISON;
    char name[LABEL_NAME_LEN] = {};
};


const char* OUTPUT_FILE = "code.txt";
const int COMMAND_LEN = 20;
const char* SIGNATURE = "ASS.300";
const int LABELS_NUM = 20;

struct Commands_Info
{
    char * buffer;
    char ** lines;
    size_t size;
    size_t lines_amount;
};


enum Errors
{
    POP_TO_IMM_CONST = 1,
    NO_SOURCE_FILE,
    REGISTER_ERROR,
    UNKNOWN_ARGUMENT,
    ZERO_POINTER,
    MANY_LABELS,
    JMP_ERROR,
    ARGV_ERROR,
};

const char* Errors_Names[] =   {"It's not an error, just let it be here",
                                "Trying to pop to immediate constant",
                                "No source file found",
                                "Cringe register name",
                                "Unknow argument",
                                "Zero_pointer",
                                "Too many labels already used",
                                "Don't understand where to jump",
                                "Write 2 arguments after the assembler.exe shows the input and output files names"
                                };

void Read_From_File(struct Commands_Info* text, char* inp_file_name);
void Lines_Connecter(struct Commands_Info* text);
void Get_arg(char* Line, char* code, size_t* pointer, int name, int line);
void add_comm_name(char* code, size_t* pointer, int name, int have_mem, int have_reg, int have_imm);
void add_imm_const(char* code, size_t* pointer, int value);
void add_reg(char* code, size_t* pointer, char* registr, int line);
void Get_Code(struct Commands_Info* text, struct label* labels, char* output_file_name);
void Reach_Space(char** ptr);
void Skip_Spaces(char** ptr);
void Out_Code(char* code, char* output_file_name, size_t pointer);
void find_label(char* name, struct label* labels, char* code, size_t* pointer);
void Errors_Processing(int Err, int lline = 0);