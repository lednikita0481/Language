#ifndef INCLUDED_J_H
#define INCLUDED_J_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>


#define DEF_CMD(name, num, arg, ...)\
        byte_code_##name = num,  

enum byte_code 
{
    #include "cmd.h"
    byte_code_mov
};

#undef DEF_CMD

struct Argument
{
    bool has_arg;
    int arg;
};

struct Byte_Code_Node
{
    int ip;
    byte_code command_type;
    bool processed;
    Argument mem_arg;
    Argument reg_arg;
    Argument imm_arg;
    Byte_Code_Node* next_node;
    Byte_Code_Node* prev_node;
};

struct Byte_Code_Nodes_List
{
    Byte_Code_Node* first_node;
    int nodes_amount;
};

struct Text
{
    char* buffer;
    size_t size;
    int ip;
};

enum Registers
{
    rax,
    rcx,
    rdx,
    rbx,
    rbp,
    rsp,
    rsi,
    rdi,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15
};

enum x86_Commands : u_int64_t
{
    // mov r_x, imm
    MOV_REG_IMM = 0xb848,                // "|" with shifted by 8 reg mask (till rdi)
    MOV_R15_IMM = 0xba49,

    MOV_REG_REG = 0xc08948,             // first reg - "|" with shifted by 19 mask
                                        // second reg - "|" with shifted by 16 mask

    // mov r_x, [r15 + offset]
    MOV_REG_R15_OFFSET = 0x878b49,      // "|" with shifted left by 19 reg
                                        //followed by int offset

    // mov [r15 + offset], r_x
    MOV_R15_OFFSET_REG = 0x878949,      // the same

    PUSH_REG = 0x50,                     // "|" with reg mask

    // push [r15 + offset]
    PUSH_R15_OFFSET = 0xb7ff41,          // followed by int offset

    POP_REG = 0x58,                     // "|" with reg mask

    POP_R13 = 0x5d41,
    PUSH_R13 = 0x5541,

    // pop [r15 + offset]
    POP_R15_OFFSET = 0x878f41,          // followed by int offset

    ADD_REG_REG = 0xc00148,             // first reg - "|" with shifted by 19 mask
                                        // second reg - "|" with shifted by 16 mask

    SUB_REG_REG = 0xc02948,             // first reg - "|" with shifted by 19 mask
                                        // second reg - "|" with shifted by 16 mask

    IMUL_REG_REG = 0xc0af0f48,          // first reg - "|" with shifted by 27 mask
                                        // second reg - "|" with shifted by 24 mask

    CQO = 0x9948,                       // signed rax -> (rdx, rax)

    IDIV_REG = 0xf8f748,                // "|" with reg mask shifted by 16 

    CVTSI2SD_XMM0_RAX = 0xc02a0f48f2,   // mov xmm0, rax
    CVTSD2SI_RAX_XMM0 = 0xc02d0f48f2,   // mov rax, xmm0

    DIVPD_XMM0_XMM1 = 0xc15e0f66,       // xmm0/xmm1 -> xmm0
    MULPD_XMM0_XMM1 = 0xc1590f66,       // xmm0*xmm1 -> xmm0

    CVTSI2SD_XMM1_RAX = 0xc82a0f48f2,   // mov xmm1, rax

    LEA_RDI_R15_OFFSET = 0xbf8d49,      // lea rdi, [r15 + offset]

    x86_RET = 0xc3,     

    x86_JMP = 0xe9,

    x86_COND_JMP = 0x000f,              // with cond mask shifted by 8

    x86_CALL = 0xe8,

    CMP_RAX_RBX = 0xd83948,

    SQRTPD_XMM0_XMM0 = 0xc0510f66,      // calculates sqrt it xmm0 

    LEA_RDI_RSP = 0x00247C8D48,         // lea rdi, [rsp]
    PUSH_ALL = 0x505152535241,          // push r10 - rax - ... - rdx
    POP_ALL  = 0x5A415B5A5958,          // pop rdx - ... rax - r10

    MOV_RBP_RSP = 0xE48949,
    MOV_RSP_RBP = 0xE4894C,
    AND_RSP_FF = 0xF0E48348,

    MOV_XMM_RSP = 0x002400100FF2,       // movsd xmm0-4, [rsp]
};

enum x86_Commands_Size
{
    SIZE_MOV_REG_IMM = 2,
    SIZE_MOV_R15_IMM = 2,
    SIZE_MOV_REG_REG = 3,
    SIZE_MOV_REG_R15_OFFSET = 3,
    SIZE_MOV_R15_OFFSET_REG = 3,
    SIZE_PUSH_REG = 1,
    SIZE_PUSH_R15_OFFSET = 3,
    SIZE_POP_REG = 1,
    SIZE_POP_R15_OFFSET = 3,
    SIZE_ADD_REG_REG = 3,
    SIZE_SUB_REG_REG = 3,
    SIZE_IMUL_REG_REG = 4,
    SIZE_CQO = 2,
    SIZE_IDIV_REG = 3,
    SIZE_CVTSI2SD_XMM0_RAX = 5,
    SIZE_CVTSD2SI_RAX_XMM0 = 5,
    SIZE_DIVPD_XMM0_XMM1 = 4,
    SIZE_MULPD_XMM0_XMM1 = 4,
    SIZE_CVTSI2SD_XMM1_RAX = 5,
    SIZE_LEA_RDI_R15_OFFSET = 3,
    SIZE_x86_RET = 1,
    SIZE_x86_JMP = 1,
    SIZE_x86_COND_JMP = 2,
    SIZE_x86_CALL = 1,
    SIZE_CMP_RAX_RBX = 3,
    SIZE_SQRTPD_XMM0_XMM0 = 4,
    SIZE_LEA_RDI_RSP = 5,
    SIZE_PUSH_ALL = 6,
    SIZE_POP_ALL = 6,
    SIZE_MOV_RBP_RSP = 3,
    SIZE_MOV_RSP_RBP = 3,
    SIZE_AND_RSP_FF = 4,
    SIZE_MOV_XMM_RSP = 6,
    SIZE_POP_R13 = 2,
    SIZE_PUSH_R13 = 2,
};

enum COND_JMPS
{
    JE_MASK = 0x84,
    JNE_MASK = 0x85,
    JG_MASK = 0x8f,
    JGE_MASK = 0x8d,
    JL_MASK = 0x8c,
    JLE_MASK = 0x8e,
};

struct x86_Node
{
    u_int64_t command;
    x86_Commands command_without_mask;
    int command_size;
    int old_ip;
    int new_ip;
    bool has_imm_arg;
    bool is_in_call;
    bool is_out_call;
    int imm_arg;

    x86_Node* next_node;
    x86_Node* prev_node;
};

struct x86_Nodes_List
{
    x86_Node* first_node;
    int size;
    x86_Node* current_using;
    int code_size;
};

enum ERRORS
{
    NO_ERROR,
    FILE_INPUT_ERROR,
    WRONG_SIGNATURE,
    WRONG_SIZE,
    UNKNOWN_COMMAND,
    NO_JIT_COMMAND_YET,
    NO_LABEL_FOUND,
    MPROTECT_ERROR,
};


void Error_Occures(ERRORS error, int ip = -1, char byte = 0);
void JIT_load(x86_Nodes_List* x86_list);

#endif