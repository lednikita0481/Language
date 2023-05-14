#include <stdio.h>

struct CPU
{
    char* code;
    int*   regs;
    int*   RAM;
    int     ip;
    int pointer_max;
    Stack stk = {};
    Stack call_adress_stk = {};
};


#define DEF_CMD(name, num, arg, ...)\
        cmd_##name = num,  
enum Commands
{
    #include "../cmd.h"
};
#undef DEF_CMD

const int SIGNATURE_LEN = 8;
const char* NEEDED_SIGNATURE = "ASS.300";
const int MEMORY_SIZE = 5000;
const int WINDOW_WIDTH = 100;
const int REG_SIZE = 10;
const char CMD_MASK = 31; //11111
const int ACCURACY = 1000;

enum CPU_Errors
{
    NO_CODE_FILE = 1,
    WRONG_SIGNATURE,
    UNKNOWN_COMMAND,
    MEM_SEG_FAULT,
    DIV_BY_ZERO,
    ARGV_ERROR
};

const char* Errors_Names[] =   {"It's not an error, just let it be here",
                                "Code file opening error",
                                "Signature Error. Recompile or buy new processor",
                                "I don't know such command, you have a broken file",
                                "Memory oversize",
                                "impossible to divide by zero",
                                "Write 1 argument after the CPU.exe shows the input file name"
                                };

enum shift
{
    imm_const = 5,
    reg       = 6,
    memory    = 7
};

void Get_Code(struct CPU* cpu, char* inp_file_name);
void Check_Signature(FILE* code_file);
void CPU_Ctor(struct CPU* cpu);
void Do_What_Said(struct CPU* cpu);
int* Get_arg(struct CPU* cpu);
void Print_Memory(struct CPU* cpu);
void ClearScreen();
void Errors_Processing(int Err);
