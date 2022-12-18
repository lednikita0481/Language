#include <stdio.h>
#include <stdlib.h>

const char* USER_FILE_NAME = "code.txt";

struct Text
{
    char* buff;
    int symbols;
    int line;
};

enum Types
{
    VALUE_TYPE,
    VAR_TYPE,
    SPECIAL_TYPE,
    NAME_TYPE
};

enum Language_Types
{
    NOT_LANG_TYPE,
    SKIP,

    //арифметика
    ADD,//
    SUB,//
    DIV,//
    MUL,//
    POW,//
    SIN,//
    COS,//

    //сравнение
    EQ_EQ,//
    LESS_EQ,//
    MORE_EQ,//
    LESS, //
    MORE,//
    NOT_EQ,//

    EQ,

    //логика
    VAR,
    IF,
    ELSE, 
    WHILE,
    START_EXP,//
    END_EXP,//
    END_AC,
    BEGIN_COND,//
    END_COND,//
    FUNC,
    CALL_FUNCTION,
    RET,
    SCAN, 
    OUT,
    AND,
    OR,

    //help types
    NIL,
    ST,
    PARAM
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
    int code_line;
    int num;
};

struct Function
{
    Node* node;
    char* name;
    size_t hash;
};

Function FUNCTIONS[100] = {};
int F_NUM = 0;

int IMAGE_NUM = 0;


void File_Read(Text* text);
size_t Calculate_Hash(void* Structure, size_t size_bites);
void Syntax_Error(int current_line);
void Tokenise(Node** nodes, Text* text);
Node* Create_Node(Types type, double dval, Language_Types l_type, char* name, char* call_func_name, Node* left, Node* right, Node* parent, int code_line);
void Skip_Spaces_Comments(Text* text, int* pointer, int* current_line);
int Next_Word_Len(Text* text, int* pointer);
int isalpharus(char p);
char* Get_Lim_Len_String(char* to, const char* from, const int lendth);
double Get_Num(const char* str, int* pointer);
void Token_Dump(Node** nodes, int amount);
const char* Spec_Type_Name(Language_Types type);
void Proga_Dump();
void Recursive_Node_Dump(Node* node, FILE* Dump_File);

void Lexic_Error(int line);
void Lexical_Tree_Connector(Node** nodes, int Tokens_Amount);
void Get_Func_Name(Node** nodes, int* cur_token, Node* func_node);
void Get_Parametrs(Node** nodes, int* cur_token, Node* func_node, Node* name_node);
Node* Get_Param(Node** nodes, int* cur_token, Node* func_node);
Node* Get_Statement_Exp(Node** nodes, int* cur_token);
Node* Get_Statement(Node** nodes, int* cur_token);
Node* Get_Action(Node** nodes, int* cur_token);
Node* GetE(Node** nodes, int* cur_token);
Node* GetT(Node** nodes, int* cur_token);
Node* GetW(Node** nodes, int* cur_token);
Node* GetP(Node** nodes, int* cur_token);
Node* GetN(Node** nodes, int* cur_token);

Node* Main_Node = NULL;



//-----------------------------------------DUMP---------------------------------

#define DUMP_TOKEN_FILE_NAME "token1.gv"
#define DUMP_PROGA_FILE_NAME "proga1.gv"
const char png[5] = ".png";


#define Dump(str...)                                                 \
    fprintf(Dump_File, str);                                               \
    fflush(Dump_File)

FILE* Open_Dump(const char* name) 
{
    FILE* Dump = fopen(name, "w");
    if (Dump == NULL) printf("ERROR, CAN'T OPEN DUMP FILE\n");
    return Dump;
}
