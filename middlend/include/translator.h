#include <stdio.h>
#include <stdlib.h>

enum Types
{
    VALUE_TYPE,
    VAR_TYPE,
    SPECIAL_TYPE,
    NAME_TYPE
};

enum Language_Types
{
    NOT_LANG_TYPE,//0

    //арифметика
    ADD,//1
    SUB,//2
    DIV,//3
    MUL,//4
    POW,//5
    SIN,//6
    COS,//7

    //сравнение
    IS_EE,//8
    IS_BE,//9
    IS_GE,//10
    IS_BT, //11
    IS_GT,//12
    IS_NE,//13
    EQ,//14

    //логика
    VAR,//15
    IF,//16
    ELSE, //17
    WHILE,//18
    FUNC,//19
    VOID,//20
    TYPE,//21
    CALL,//22
    RET,//23
    SCAN, //24
    OUT,//25
    AND,//26
    OR,//27

    //help types
    NIL,//28
    ST,//29
    PARAM//30
};

union lang_value
{
    double dval;
    Language_Types l_type;
    char*  name;
};


struct Node
{
    Types type;
    lang_value value;
    char* call_func_name;
    Node* left;
    Node* right;
    Node* parent;
    int num;
};

int N_NUM = 0;

int MAX_LEN_ELEM = 100;

FILE* CODE_FILE = NULL;
const char* DEFAULT_CODE_FILE_NAME = "code.txt";
FILE* ASM_FILE = NULL;
const char* DEFAULT_ASM_FILE_NAME = "code.asm";

Node* MAIN_NODE = NULL;

char* VARIABLES[100] = {};
int CUR_VAR = 0;

int IF_LABELS = 0;


struct code_string
{
    char* name;
    int lendth;
    Language_Types l_type;
};

const int COMMAND_AMOUNT = 30;

code_string COMMANDS[] = 
                        {
                            "ST", 2, ST,
                            "IF", 2, IF,
                            "ELSE", 4, ELSE,
                            "NIL", 3, NIL,
                            "VAR", 3, VAR,
                            "WHILE", 5, WHILE,
                            "FUNC", 4, FUNC,
                            "RET", 3, RET,
                            "CALL", 4, CALL,
                            "PARAM", 5, PARAM,
                            "EQ", 2, EQ,
                            "VOID", 4, VOID,
                            "TYPE", 4, TYPE,
                            "ADD", 3, ADD,
                            "SUB", 3, SUB,
                            "DIV", 3, DIV,
                            "MUL", 3, MUL,
                            "POW", 3, POW,
                            "SIN", 3, SIN,
                            "COS", 3, COS,
                            "AND", 3, AND,
                            "OR", 2, OR,
                            "SCAN", 4, SCAN,
                            "OUT", 3, OUT,
                            "IS_EE", 5, IS_EE,
                            "IS_BE", 5, IS_BE,
                            "IS_GE", 5, IS_GE,
                            "IS_BT", 5, IS_BT,
                            "IS_GT", 5, IS_GT,
                            "IS_NE", 5, IS_NE,
                        };


void Open_Read_File(const int argc, const char* argv[], char* text);
void Close_Read_File();
void Open_Write_File(const int argc, const char* argv[]);
void Close_Write_File();
Node* Create_Node(Types type, double dval, Language_Types l_type, char* name, char* call_func_name, Node* left, Node* right, Node* parent);
Node* Scan_Node();
Language_Types Chech_L_Type(char* line);
int Get_Var(char* var);
void Start_Asm_Dump();
void Asm_Node(Node* node, Language_Types previous_node_type = NOT_LANG_TYPE);
void Asm_Condition(Node* node, bool reversed, int begin_if, int begin_else);

const char* Spec_Type_Name(Language_Types type);
void Proga_Dump();
void Recursive_Node_Dump(const Node* node, FILE* Dump_File);



#define DUMP_PROGA_FILE_NAME "proga1.gv"
const char png[5] = ".png";


#define Dump(str...)                                                 \
    fprintf(Dump_File, str);                                               \
    fflush(Dump_File)

#define PRINT_ASM(str...)\
    fprintf(ASM_FILE, str);\
    fprintf(ASM_FILE, "\n");

FILE* Open_Dump(const char* name) 
{
    FILE* Dump = fopen(name, "w");
    if (Dump == NULL) printf("ERROR, CAN'T OPEN DUMP FILE\n");
    return Dump;
}
