#include "include/Translation.h"
#include "include/dump.h"

const int NATIVE_MEMORY_SIZE = 1000;
int variables[NATIVE_MEMORY_SIZE] = {};
int current_var = 8;
const int X86_MEMORY_CELL = 8;
const int ACCURACY = 1000;

const int MAX_LABLES_AMOUNT = 100;

const int CODE_DATA_SPREAD = 500;

x86_Nodes_List* Translate(Byte_Code_Nodes_List* byte_list)
{
    x86_Nodes_List* x86_list = (x86_Nodes_List*) calloc(1, sizeof(x86_Nodes_List));
    x86_list->first_node = (x86_Node*) calloc(1, sizeof(x86_Node));
    x86_list->current_using = x86_list->first_node;

    label lables[MAX_LABLES_AMOUNT] = {};

    int code_size_int = 0;
    int* code_size = &code_size_int;

    SET_COMAND(MOV_R15_IMM);
    SET_IMM_ARG(0);
    NEXT_NODE();

    //SET_COMAND(POP_R13);
    //NEXT_NODE();

    Byte_Code_Node* byte_node = byte_list->first_node;
    for (int i = 0; i < byte_list->nodes_amount; i++)
    {
        switch (byte_node->command_type)
        {
            case byte_code_push:
                Push_Handler(byte_node, x86_list, code_size);
                break;

            case byte_code_pop:
                Pop_Handler(byte_node, x86_list, code_size);
                break;

            case byte_code_add:
            case byte_code_sub:
            case byte_code_mul:
            case byte_code_div:
                Add_Sub_Imul_Idiv_Handler(byte_node, x86_list, code_size);
                break;
            
            case byte_code_sqrt:
                sqrt_Handler(byte_node, x86_list, code_size);
                break;

            case byte_code_jmp:
            case byte_code_call:
                Jmp_Call_Handler(byte_node, x86_list, lables, code_size);
                break;

            case byte_code_ja:
            case byte_code_jae:
            case byte_code_jb:
            case byte_code_jbe:
            case byte_code_je:
            case byte_code_jne:
                CondJmp_Handler(byte_node, x86_list, lables, code_size);
                break;

            case byte_code_hlt:
            case byte_code_ret:
                Ret_Handler(byte_node, x86_list, code_size);
                break;

            case byte_code_in:
                In_Handler(byte_node, x86_list, code_size);
                break;

            case byte_code_out:
                Out_Handler(byte_node, x86_list, code_size);
                break;
            
            default:
                Error_Occures(NO_JIT_COMMAND_YET);
                break;
        }

        byte_node = byte_node->next_node;
    }

    SET_COMAND(PUSH_R13);
    NEXT_NODE();

    x86_list->code_size = *code_size;
    Update_Vars_And_Labels(x86_list, lables);
    return x86_list;
}


// NOW LANGUAGE TO ASM TRANSLATOR WORKS ONLY IN REGIME WITH PUSH & POP CONST OR IMMEDIATE
// NO REGISTERS IN USE. IMPROVE THIS FUNCTION AFTER/IF CHANGING!
void Push_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size)
{
    if (byte_node->mem_arg.has_arg)
    {
        SET_COMAND(PUSH_R15_OFFSET);
        SET_IMM_ARG_MEM();                      // после полной трансляции обновить адреса на + code size
        SET_OLD_IP();
    }

    else
    {
        SET_COMAND(MOV_REG_IMM, | (rax << 8));
        SET_IMM_ARG(byte_node->imm_arg.arg);
        SET_OLD_IP();
        NEXT_NODE();
        SET_COMAND(PUSH_REG, | rax);
    }

    NEXT_NODE();
}

void Pop_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size)
{
    if (byte_node->mem_arg.has_arg)
    {
        SET_COMAND(POP_R15_OFFSET);
        SET_IMM_ARG_MEM();
    }

    else
    {
        SET_COMAND(POP_REG, | (byte_node->reg_arg.arg));
    }

    SET_OLD_IP();
    NEXT_NODE();
}

void Add_Sub_Imul_Idiv_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size)
{
    SET_COMAND(POP_REG, | rbx);
    SET_OLD_IP();
    NEXT_NODE();

    SET_COMAND(POP_REG, | rax);
    NEXT_NODE();

    if (byte_node->command_type == byte_code_add) {SET_COMAND(ADD_REG_REG, | (rax << 16) | (rbx << 19));}
    else if (byte_node->command_type == byte_code_sub) {SET_COMAND(SUB_REG_REG, | (rax << 16) | (rbx << 19));}
    else if (byte_node->command_type == byte_code_mul)
    {
        SET_COMAND(IMUL_REG_REG, | (rbx << 24) | (rax << 27));
        NEXT_NODE();

        SET_COMAND(MOV_REG_IMM, | (rdi << 8));
        SET_IMM_ARG(ACCURACY);
        NEXT_NODE();

        SET_COMAND(CQO);
        NEXT_NODE();

        SET_COMAND(IDIV_REG, | (rdi << 16));
        NEXT_NODE();

        SET_COMAND(MOV_REG_IMM, | (rdx << 8));
        SET_IMM_ARG(0);
        NEXT_NODE();

        SET_COMAND(MOV_REG_IMM, | (rdi << 8));
        SET_IMM_ARG(0);
    }

    else
    {
        SET_COMAND(MOV_REG_IMM, | (rdi << 8));
        SET_IMM_ARG(ACCURACY);
        NEXT_NODE();

        SET_COMAND(IMUL_REG_REG, | (rax << 27) | (rdi << 24));
        NEXT_NODE();

        SET_COMAND(CQO);
        NEXT_NODE();

        SET_COMAND(IDIV_REG, | (rbx << 16));
        NEXT_NODE();

        SET_COMAND(MOV_REG_IMM, | (rdx << 8));
        SET_IMM_ARG(0);
        NEXT_NODE();

        SET_COMAND(MOV_REG_IMM, | (rdi << 8));
        SET_IMM_ARG(0);
    }
    NEXT_NODE();

    SET_COMAND(PUSH_REG, | rax);
    NEXT_NODE();
}

void sqrt_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size)
{
    SET_COMAND(POP_REG, | rax);
    SET_OLD_IP();
    NEXT_NODE();

    SET_COMAND(CVTSI2SD_XMM0_RAX);
    NEXT_NODE();

    SET_COMAND(MOV_REG_IMM, | (rax << 8));
    SET_IMM_ARG(ACCURACY);
    NEXT_NODE();

    SET_COMAND(CVTSI2SD_XMM1_RAX);
    NEXT_NODE();

    SET_COMAND(DIVPD_XMM0_XMM1);
    NEXT_NODE();

    SET_COMAND(SQRTPD_XMM0_XMM0);
    NEXT_NODE();

    SET_COMAND(MULPD_XMM0_XMM1);
    NEXT_NODE();

    SET_COMAND(CVTSD2SI_RAX_XMM0);
    NEXT_NODE();

    SET_COMAND(PUSH_REG, | rax);
    NEXT_NODE();
}

void Add_Label(label* labels, int old_ip)
{
    int i = 0;
    for (; i < MAX_LABLES_AMOUNT; i++)
    {
        if (labels[i].old_ip == -1) break;
        if (labels[i].old_ip == old_ip) return;
    }

    labels[i].old_ip = old_ip;
}

void Jmp_Call_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, label* labels, int* code_size)
{
    if (byte_node->command_type == byte_code_jmp) 
    {
        SET_COMAND(x86_JMP);
    }
    else 
    {
        SET_COMAND(x86_CALL);
    }

    SET_OLD_IP();
    SET_IMM_ARG(byte_node->imm_arg.arg);
    Add_Label(labels, byte_node->imm_arg.arg);
    NEXT_NODE();    
}

void CondJmp_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, label* labels, int* code_size)
{
    SET_COMAND(POP_REG, | rbx);
    SET_OLD_IP();
    NEXT_NODE();

    SET_COMAND(POP_REG, | rax);
    NEXT_NODE();

    SET_COMAND(CMP_RAX_RBX);
    NEXT_NODE();

    //SET_COMAND(PUSH_REG, | rax);
    //NEXT_NODE();

    //SET_COMAND(PUSH_REG, | rbx);
    //NEXT_NODE();

    switch (byte_node->command_type)
    {
        case byte_code_jb:
            SET_COMAND(x86_COND_JMP, | (JL_MASK << 8));
            break;
        
        case byte_code_jbe:
            SET_COMAND(x86_COND_JMP, | (JLE_MASK << 8));
            break;

        case byte_code_ja:
            SET_COMAND(x86_COND_JMP, | (JG_MASK << 8));
            break;

        case byte_code_jae:
            SET_COMAND(x86_COND_JMP, | (JGE_MASK<< 8));
            break;
        
        case byte_code_je:
            SET_COMAND(x86_COND_JMP, | (JE_MASK << 8));
            break;

        case byte_code_jne:
            SET_COMAND(x86_COND_JMP, | (JNE_MASK << 8));
            break;
        
        default:
            break;
    }
    SET_IMM_ARG(byte_node->imm_arg.arg);
    NEXT_NODE();
    Add_Label(labels, byte_node->imm_arg.arg);
}

void Ret_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size)
{
    SET_COMAND(x86_RET);
    SET_OLD_IP();
    NEXT_NODE();
}

void In_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size)
{

    SET_COMAND(LEA_RDI_R15_OFFSET);
    SET_IMM_ARG(0);
    SET_OLD_IP();
    x86_list->current_using->is_in_call = true;
    NEXT_NODE();

    SET_COMAND(PUSH_ALL);
    NEXT_NODE();

    SET_COMAND(MOV_RBP_RSP);
    NEXT_NODE();

    SET_COMAND(AND_RSP_FF);
    NEXT_NODE();

    SET_COMAND(x86_CALL);
    x86_list->current_using->is_in_call = true;
    SET_IMM_ARG(0);
    NEXT_NODE();

    SET_COMAND(MOV_RSP_RBP);
    NEXT_NODE();

    SET_COMAND(POP_ALL);
    NEXT_NODE();

    SET_COMAND(PUSH_R15_OFFSET);
    x86_list->current_using->is_in_call = true;
    SET_IMM_ARG(0);
    NEXT_NODE();
}

void Out_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size)
{
    SET_COMAND(POP_REG, | rdi);
    SET_OLD_IP();
    NEXT_NODE();

    SET_COMAND(PUSH_ALL);
    NEXT_NODE();

    SET_COMAND(MOV_RBP_RSP);
    NEXT_NODE();

    SET_COMAND(AND_RSP_FF);
    NEXT_NODE();

    SET_COMAND(x86_CALL);
    x86_list->current_using->is_out_call = true;
    SET_IMM_ARG(0);
    NEXT_NODE();

    SET_COMAND(MOV_RSP_RBP);
    NEXT_NODE();

    SET_COMAND(POP_ALL);
    NEXT_NODE();
}

void Update_Label(label* labels, int old_ip, int new_ip)
{
    for (int i = 0; i < MAX_LABLES_AMOUNT; i++)
    {
        if (labels[i].old_ip == old_ip)
        {
            labels[i].new_ip = new_ip;
            break;
        }
    }
}

int Get_Label_NewIp(label* labels, int old_ip)
{
    for (int i = 0; i < MAX_LABLES_AMOUNT; i++)
    {
        if (labels[i].old_ip == old_ip) return labels[i].new_ip;
    }

    //printf("Holly shit wtf is here I don't know but old ip is %d\n", old_ip);
    Error_Occures(NO_LABEL_FOUND);
}

void Update_Vars_And_Labels(x86_Nodes_List* x86_list, label* labels)
{
    int cur_code_size = 0;
    x86_list->current_using = x86_list->first_node;
    for (int i = 0; i < x86_list->size; i++)
    {
        if (x86_list->current_using->old_ip != -1) Update_Label(labels, x86_list->current_using->old_ip, cur_code_size);

        if (x86_list->current_using->command_without_mask == PUSH_R15_OFFSET || x86_list->current_using->command == POP_R15_OFFSET)
            x86_list->current_using->imm_arg += CODE_DATA_SPREAD + x86_list->code_size;
        
        if ((x86_list->current_using->command_without_mask == PUSH_R15_OFFSET || x86_list->current_using->command_without_mask == LEA_RDI_R15_OFFSET)
            && x86_list->current_using->is_in_call)
        {
            x86_list->current_using->imm_arg = x86_list->code_size + CODE_DATA_SPREAD - 16;
        }

        x86_list->current_using->new_ip = cur_code_size;

        cur_code_size += x86_list->current_using->command_size;

        if (x86_list->current_using->has_imm_arg) 
        {
            if ((x86_list->current_using->command & MOV_REG_IMM) == MOV_REG_IMM || x86_list->current_using->command == MOV_R15_IMM)
                cur_code_size += sizeof(long int);
            else cur_code_size += sizeof(int);
        }

        x86_list->current_using = x86_list->current_using->next_node;
    }

    x86_list->code_size = cur_code_size;

    //x86_List_Dump(x86_list);

    x86_list->current_using = x86_list->first_node;

    for (int i = 0; i < x86_list->size; i++)
    {
        if ((x86_list->current_using->command_without_mask == x86_CALL
            || x86_list->current_using->command_without_mask == x86_JMP
            || x86_list->current_using->command_without_mask == x86_COND_JMP)
            && (!x86_list->current_using->is_in_call) && (!x86_list->current_using->is_out_call))
        {
            //printf("trying to find label for a node with new ip = %d and command %s\n", x86_list->current_using->new_ip, Get_x86_Command_Name(x86_list->current_using->command_without_mask));
            //printf("\n%d %d\n\n\n", Get_Label_NewIp(labels, x86_list->current_using->imm_arg), x86_list->current_using->new_ip);
            x86_list->current_using->imm_arg = Get_Label_NewIp(labels, x86_list->current_using->imm_arg) - (x86_list->current_using->new_ip + x86_list->current_using->command_size + sizeof(int));
        }

        x86_list->current_using = x86_list->current_using->next_node;
    }
}   