#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <locale.h>

#include "include/headers/language.h"
#include "include/headers/dictionary.h"
#include "include/headers/DSL.h"

int main()
{
    setlocale(LC_ALL, "Rus");
    Text text = {};
    File_Read(&text);
    Node** nodes = (Node**) calloc(text.symbols, sizeof(Node*));
    Tokenise(nodes, &text);
    Standart_Dump();
}

//-------------------------------Help Functions-----------------------------------------------------------------------------------------------------------------

void File_Read(Text* text)
{
    FILE* file = fopen(USER_FILE_NAME, "r");
    struct stat buff = {};
    fstat(fileno(file), &buff);
    int size = buff.st_size;

    text->buff = (char*) calloc(size, sizeof(char));

    text->symbols = fread(text->buff, sizeof(char), size, file);

    fclose(file);
}

void Syntax_Error(int current_line)
{
    printf("ERROR! You have an error in line %d\n", current_line);
    exit(66);
}

void Skip_Spaces_Comments(Text* text, int* pointer, int* current_line)
{
    while((*(text->buff+(*pointer)) == ' ' || *(text->buff+(*pointer)) == '\t') && *pointer < text->symbols) (*pointer)++;

    if (*(text->buff+(*pointer)) == '/' && *(text->buff+(*pointer) + 1) == '/')
    {
        while (*pointer < text->symbols && *(text->buff+(*pointer)) != '\n')
        {
            (*pointer)++;
        }
        (*current_line)++;
    }

    if (*(text->buff+(*pointer)) == '\n')
    {
        (*current_line)++;
        (*pointer)++;
    }

    while((*(text->buff+(*pointer)) == ' ' || *(text->buff+(*pointer)) == '\t') && *pointer < text->symbols) (*pointer)++;
}

int Next_Word_Len(Text* text, int* pointer)
{
    int i = 0;
    int p = *pointer;

    while(isalpharus(*(text->buff + p)))
    {
        p++;
        i++;
    }

    return i;
}

int isalpharus(char p)
{
    if ((p <= 'я' && p >= 'а') || (p <= 'Я' && p >= 'А') || isalnum(p)) return 1;
    else return 0;
}

char* Get_Lim_Len_String(char* to, const char* from, const int lendth)
{
    for (int i = 0; i < lendth; i++)
    {
        to[i] = from[i];
    }

    printf("name is %s\n", to);
    return to;
}

double Get_Num(const char* str, int* pointer)
{
    double val = 0;

    while ('0' <= *str && *str <= '9')
    {
        val = val*10 + *str - '0';
        str++;
        (*pointer)++;
    }
    if (*str == '.')
    {
        str++;
        (*pointer)++;
        int i = 1;
        while ('0' <= *str && *str <= '9')
        {
            double add = (double)(*str - '0');
            for (int j = 0; j < i; j++) add /= 10;
            val = val + add; 
            str++;
            (*pointer)++;
            i++;
        }
    }

    return val;
}

size_t Calculate_Hash(void* Structure, size_t size_bites)
{
    char* structure = (char*)Structure;
    size_t control_sum = 0xDED9EBE1;
    for (size_t i = 0; i < size_bites; i++)
    {
        control_sum += i * structure[i];
    }

    return (((control_sum << 5) + control_sum) + 1);
}


//--------------------------------Tokeniser---------------------------------------------------------------------------------------------------------------------------


void Tokenise(Node** nodes, Text* text)
{
    int current_line = 1;
    int current_token = 0;
    int pointer = 0;

    while (pointer < text->symbols)
    {
        printf("Pointer is %d, line is %d %10s\n", pointer, current_line, text->buff+pointer);
        bool found_node = false;

        for (int i = 0; i < COMMANDS_NUMBER; i++)
        {
            if (strncmp(text->buff+pointer, COMMANDS[i].name, COMMANDS[i].lendth) == 0)
            {
                found_node = true;
                nodes[current_token] = Create_Spec(COMMANDS[i].l_type, current_line);
                current_token++;
                pointer += COMMANDS[i].lendth;
                break;
            }
        }

        SSaC();

        if (found_node)
        {
            if (nodes[current_token-1]->value.l_type == VAR)
            {
                int name_len = Next_Word_Len(text, &pointer);
                printf("name len is %d\n\n\n", name_len);
                char* name = (char*) calloc(name_len+1, sizeof(char));
                Get_Lim_Len_String(name, (text->buff + pointer), name_len);

                Syntax_Assert(!isdigit(name[0]));

                nodes[current_token] = Create_Var(name, current_line);

                current_token++;
                pointer += name_len;

                SSaC();
            }

            else if (nodes[current_token-1]->value.l_type == EQ)
            {
                double val = Get_Num(text->buff + pointer, &pointer);
                nodes[current_token] = Create_Value(val, current_line);

                current_token++;
                SSaC();
            }

            else if (nodes[current_token-1]->value.l_type == FUNC_D || nodes[current_token-1]->value.l_type == FUNC_V)
            {
                int name_len = Next_Word_Len(text, &pointer);
                printf("len is %d\n", name_len);
                char* name = (char*) calloc(name_len+1, sizeof(char));
                Get_Lim_Len_String(name, (text->buff + pointer), name_len);

                Syntax_Assert(!isdigit(name[0]));

                nodes[current_token] = Create_Name(name, current_line);

                current_token++;
                pointer += name_len;

                SSaC();
                Syntax_Assert(*(text->buff + pointer) == '(');
            }
        }

        else
        {
            int old_p = pointer;
            double dbl = Get_Num(text->buff + pointer, &pointer);

            if (pointer > old_p)
            {
                nodes[current_token] = Create_Value(dbl, current_line);

                current_token++;
                SSaC();
            }

            else
            {
                int name_len = Next_Word_Len(text, &pointer);
                char* name = (char*) calloc(name_len+1, sizeof(char));
                Get_Lim_Len_String(name, (text->buff + pointer), name_len);
                pointer += name_len;

                if (*(text->buff + pointer) != '(') nodes[current_token] = Create_Var(name, current_line);
                else 
                {
                    nodes[current_token] = Create_Node(SPECIAL_TYPE, 0, CALL_FUNCTION, NULL, name, NULL, NULL, NULL, current_line);
                    current_token++;
                    nodes[current_token] = Create_Var(name, current_line);
                }

                current_token++;
                SSaC();
            }
        }

        SSaC();
    }

    nodes = (Node**)realloc(nodes, (current_token+1)*sizeof(Node*));

    Token_Dump(nodes, current_token);
    printf("%d\n", current_token);
    Lexical_Tree_Connector(nodes, current_token);
    Proga_Dump();
}

Node* Create_Node(Types type, double dval, Language_Types l_type, char* name, char* call_func_name, Node* left, Node* right, Node* parent, int code_line)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));

    new_node->code_line = code_line;
    new_node->type = type;
    new_node->left = left;
    new_node->right = right;
    new_node->parent = parent;
    new_node->num = IMAGE_NUM;
    new_node->call_func_name = call_func_name;
    IMAGE_NUM++;

    if      (type == VALUE_TYPE)    new_node->value.dval = dval;
    else if (type == VAR_TYPE)      new_node->value.name = name;
    else if (type == SPECIAL_TYPE)  new_node->value.l_type = l_type;
    else if (type == NAME_TYPE)     new_node->value.name = name;

    if (type == SPECIAL_TYPE && type == CALL_FUNCTION) new_node->call_func_name = call_func_name;

    return new_node;
}

//-------------------------------------Lexical analysis----------------------------------------------------------------------------------------------------------------
void Lexic_Error(int line)
{
    printf("Lexical error in line %d!\n", line);
    exit(666);
}

void Lexical_Tree_Connector(Node** nodes, int Tokens_Amount)
{
    int cur_token = 0;

    while(cur_token < Tokens_Amount-1)
    {


        //Хочу кринжа и делаю!


        printf("node num is %d\n", nodes[cur_token]->num);
        switch (nodes[cur_token]->type)
        {
            case SPECIAL_TYPE:
                switch (nodes[cur_token]->value.l_type)
                {
                    case FUNC_D:
                    {
                        Node* func_node = nodes[cur_token];
                        printf("I'm in function node, node is %d\n", func_node->num);
                        cur_token++;
                        Get_Func_Name(nodes, &cur_token, func_node);
                        size_t hash = Calculate_Hash(func_node, sizeof(Node));
                        FUNCTIONS[F_NUM] = {func_node, func_node->left->call_func_name, hash};
                        F_NUM++;
                        cur_token++;
                        func_node->right = Get_Statement_Exp(nodes, &cur_token);
                        func_node->left->right = Create_Type();
                        cur_token++;
                        break;
                    }

                    case FUNC_V:
                    {
                        Node* func_node = nodes[cur_token];
                        printf("I'm in function node, node is %d\n", func_node->num);
                        cur_token++;
                        Get_Func_Name(nodes, &cur_token, func_node);
                        size_t hash = Calculate_Hash(func_node, sizeof(Node));
                        FUNCTIONS[F_NUM] = {func_node, func_node->left->call_func_name, hash};
                        F_NUM++;
                        cur_token++;
                        func_node->right = Get_Statement_Exp(nodes, &cur_token);
                        func_node->left->right = Create_Void();
                        cur_token++;
                        break;
                    }

                    default:
                        break;
                }
                break;
            
            default:
                break;
        }
        //*/
    }

    int i = 0;
    Main_Node = Create_St();
    Node* see_node = Main_Node;

    for (i = 0; i < F_NUM-1; i++)
    {
        see_node->left = FUNCTIONS[i].node;
        see_node->left->parent = see_node;
        see_node->right = Create_St();
        see_node->right->parent = see_node;
        see_node = see_node->right;
    }

    see_node->left = FUNCTIONS[i].node;
    see_node->left->parent = see_node;
    see_node->right = Create_Nil();
    see_node->right->parent = see_node;
}

//function creation processing __--__--__--__--__--__--__--__--__--__--

void Get_Func_Name(Node** nodes, int* cur_token, Node* func_node)
{
    Node* name_node = nodes[*cur_token];
    printf("I'm in get func name node, num is %d\n", name_node->num);

    if (name_node->type != NAME_TYPE) Lexic_Error(name_node->code_line);

    name_node->parent = func_node;
    func_node->left = name_node;
    (*cur_token)++;
    printf("lndflgndfgdnrg\n");
    Get_Parametrs(nodes, cur_token, func_node, name_node);
}

void Get_Parametrs(Node** nodes, int* cur_token, Node* func_node, Node* name_node)
{
    printf("hrhrhrhrh current token %d\n", *cur_token);
    printf("%d\n", nodes[*cur_token]->type != SPECIAL_TYPE);
    printf("%d\n", nodes[*cur_token]->value.l_type != BEGIN_COND);
    if (nodes[*cur_token]->type != SPECIAL_TYPE || nodes[*cur_token]->value.l_type != BEGIN_COND) Lexic_Error(nodes[*cur_token]->code_line);

    (*cur_token)++;

    printf("I'm in get parametrs\n");

    if (nodes[*cur_token]->type == SPECIAL_TYPE && nodes[*cur_token]->value.l_type == END_COND)
    {
        Node* nil = Create_Nil();
        nil->parent = name_node;
        name_node->left = nil;
    }

    else name_node->left =  Get_Param(nodes, cur_token, func_node);
    name_node->left->parent = name_node;

    printf("I'm in get parametrs again\n");
}

Node* Get_Param(Node** nodes, int* cur_token, Node* func_node)
{

    Node* param_node = Create_Param();
    Node* param_value_node = nodes[*cur_token];
    printf("I'm in get param node, line is %d\n", param_value_node->code_line);
    param_node->left = param_value_node;
    param_value_node->parent = param_node;

    (*cur_token)++;

    Node* next_node = nodes[*cur_token];
    if (nodes[*cur_token]->type == SPECIAL_TYPE && nodes[*cur_token]->value.l_type == END_COND)
    {
        printf("I'm in get param node end cond, num is %d\n", nodes[*cur_token]->num);
        param_node->right = Create_Nil();
        //(*cur_token)++;
    }
    else param_node->right = Get_Param(nodes, cur_token, func_node);


    return param_node;
}


//function body processing --__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__

Node* Get_Statement_Exp(Node** nodes, int* cur_token)
{
    Node* begin_node = nodes[*cur_token];
    printf("I'm in get statement exp node, num is %d\n", begin_node->num);

    if (begin_node->type != SPECIAL_TYPE || begin_node->value.l_type != START_EXP) 
    {
        Lexic_Error(begin_node->code_line);
    }
    (*cur_token)++;

    Node* return_node = Get_Statement(nodes, cur_token);

    begin_node = nodes[*cur_token];
    if (begin_node->type != SPECIAL_TYPE || begin_node->value.l_type != END_EXP) Lexic_Error(begin_node->code_line);
    printf("here!!!\n");

    return return_node;
}

Node* Get_Statement(Node** nodes, int* cur_token)
{
    Node* st_node = Create_St();

    printf("I'm in get statement node\n");

    st_node->left = Get_Action(nodes, cur_token);
    st_node->parent = st_node;
    Node* node = nodes[*cur_token];
    printf("I'm in get statement agian, num is %d\n", node->num);
    
    if (node->type == SPECIAL_TYPE && node->value.l_type == END_EXP)
    {
        printf("alalalala\n");
        st_node->right = Create_Nil();
        st_node->right->parent = st_node;
    }
    else 
    {
        st_node->right = Get_Statement(nodes, cur_token);
        st_node->right->parent = st_node;
    }

    return st_node;
}

Node* Get_Action(Node** nodes, int* cur_token)
{
    Node* see_node = nodes[*cur_token];
    printf("I'm in get action node, num is %d\n", see_node->num);

    if (IS_SPEC_TYPE(see_node) && NEEDED_NODE(see_node))
    {
        switch (see_node->value.l_type)
        {
        case VAR:
        {
            printf("it's var node\n");
            Node* var_node = see_node;
            (*cur_token)++;
            see_node = nodes[*cur_token];
            var_node->left = see_node;
            see_node->parent = var_node;

            (*cur_token)++;
            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !IS_EQ(see_node)) Lexic_Error(see_node->code_line);

            (*cur_token)++;
            var_node->right = GetE(nodes, cur_token);
            var_node->right->parent = var_node;
            
            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !IS_END_AC(see_node)) Lexic_Error(see_node->code_line);
            (*cur_token)++;
            return var_node;
            break;
        }

        case IF:
        {
            printf("it's if node\n");
            Node* if_node = see_node;
            (*cur_token)++;

            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !IS_BEGIN_COND(see_node)) Lexic_Error(see_node->code_line);

            (*cur_token)++;
            if_node->left = GetE(nodes, cur_token);
            if_node->left->parent = if_node;

            printf("herere current token is %d\n", *cur_token);
            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !(IS_END_COND(see_node))) Lexic_Error(see_node->code_line);

            (*cur_token)++;
            see_node = nodes[*cur_token];
            if_node->right = Get_Statement_Exp(nodes, cur_token);
            if_node->right->parent = if_node;

            (*cur_token)++;
            see_node = nodes[*cur_token];

            if (IS_SPEC_TYPE(see_node) && IS_ELSE(see_node))
            {
                printf("it's else node\n");
                Node* else_node = see_node;
                (*cur_token)++;
                see_node = nodes[*cur_token];

                else_node->left = if_node->right;
                else_node->left->parent = else_node;
                else_node->parent = if_node;
                if_node->right = else_node;

                else_node->right = Get_Statement_Exp(nodes, cur_token);
                else_node->right->parent = else_node;
                (*cur_token)++;
            }

            return if_node;
            break;
        }

        case WHILE:
        {
            printf("it's while node\n");
            Node* while_node = see_node;
            (*cur_token)++;

            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !IS_BEGIN_COND(see_node)) Lexic_Error(see_node->code_line);

            (*cur_token)++;
            while_node->left = GetE(nodes, cur_token);
            while_node->left->parent = while_node;

            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !(IS_END_COND(see_node))) Lexic_Error(see_node->code_line);

            (*cur_token)++;
            see_node = nodes[*cur_token];
            while_node->right = Get_Statement_Exp(nodes, cur_token);
            while_node->right->parent = while_node;
            (*cur_token)++;
            return while_node;
            break;
        }

        case CALL_FUNCTION:
        {
            printf("it's call function node\n");
            Node* call_node = see_node;
            printf("call node num is %d\n", call_node->num);
            (*cur_token)++;
            Node* func_name = nodes[*cur_token];
            printf("func name node num is %d\n", func_name->num);
            (*cur_token)++;
            func_name->parent = call_node;
            call_node->left = func_name;
            

            Get_Parametrs(nodes, cur_token, NULL, func_name);
            func_name->left->parent = func_name;
            func_name->right = Create_Nil();
            func_name->right->parent = func_name;

            (*cur_token)++;

            call_node->right = Create_Nil();
            call_node->right->parent = call_node;
            return call_node;
            break;
        }

        case RET:
        {
            printf("it's ret node\n");
            Node* ret_node = see_node;
            (*cur_token)++;

            ret_node->left = GetE(nodes, cur_token);
            ret_node->left->parent = ret_node;

            ret_node->right = Create_Nil();
            ret_node->right->parent = ret_node;

            return ret_node;
            break;
        }

        case SKIP:
        {
            printf("it's var revalue node\n");
            Node* var_node = see_node;
            (*cur_token)++;
            see_node = nodes[*cur_token];
            var_node->left = see_node;
            see_node->parent = var_node;

            (*cur_token)++;
            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !IS_EQ(see_node)) Lexic_Error(see_node->code_line);

            (*cur_token)++;
            var_node->right = GetE(nodes, cur_token);
            var_node->right->parent = var_node;
            
            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !IS_END_AC(see_node)) Lexic_Error(see_node->code_line);
            (*cur_token)++;
            return var_node;
            break;
        }

        case SCAN:
        {
            printf("it's scan node\n");
            Node* scan_node = see_node;
            (*cur_token)++;

            Get_Parametrs(nodes, cur_token, NULL, scan_node);
            (*cur_token)++;
            scan_node->left->parent = scan_node;

            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !IS_END_AC(see_node)) Lexic_Error(see_node->code_line);
            scan_node->right = Create_Nil();
            scan_node->right->parent = scan_node;
            (*cur_token)++;

            return scan_node;
            break;
        }

        case OUT:
        {
            printf("it's out node\n");
            Node* scan_node = see_node;
            (*cur_token)++;

            Get_Parametrs(nodes, cur_token, NULL, scan_node);
            (*cur_token)++;
            printf("get back from get param, cur token is %d\n", *cur_token);
            scan_node->left->parent = scan_node;

            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !(IS_END_AC(see_node))) Lexic_Error(see_node->code_line);
            scan_node->right = Create_Nil();
            scan_node->right->parent = scan_node;
            (*cur_token)++;

            return scan_node;
            break;
        }
        }
    }
}



//Numbers and algebra --__---__--__-___-__--__--_---__--__--__--_--

Node* GetE(Node** nodes, int* cur_token)
{
    printf("I'm in getE\n");
    //printf("GetE!\n");
    Node* left_node = GetU(nodes, cur_token);

    while (IS_SPEC_TYPE(nodes[*cur_token]) && (IS_ADD(nodes[*cur_token]) || IS_SUB(nodes[*cur_token])))
    {
        Node* op_node = nodes[*cur_token];
        (*cur_token)++;
        
        Node* right_node = GetU(nodes, cur_token);

        left_node->parent = op_node;
        op_node->left = left_node;
        right_node->parent = op_node;
        op_node->right = right_node;

        left_node = op_node;
    }
    
    return left_node;
}

Node* GetU(Node** nodes, int* cur_token)
{
    printf("I'm in getW\n");
    Node* left_node = GetT(nodes, cur_token);

    while (IS_SPEC_TYPE(nodes[*cur_token]) && IS_AND_OR(nodes[*cur_token]))
    {
        Node* op_node = nodes[*cur_token];
        (*cur_token)++;

        Node* right_node = GetT(nodes, cur_token);

        left_node->parent = op_node;
        op_node->left = left_node;
        right_node->parent = op_node;
        op_node->right = right_node;

        left_node = op_node;
    }
    return left_node;
}

Node* GetT(Node** nodes, int* cur_token)
{
    printf("I'm in getT\n");
    //printf("GetT!\n");
    Node* left_node = GetW(nodes, cur_token);

    while (IS_SPEC_TYPE(nodes[*cur_token]) && (IS_MUL(nodes[*cur_token]) || IS_DIV(nodes[*cur_token])))
    {
        Node* op_node = nodes[*cur_token];
        (*cur_token)++;

        Node* right_node = GetW(nodes, cur_token);

        left_node->parent = op_node;
        op_node->left = left_node;
        right_node->parent = op_node;
        op_node->right = right_node;

        left_node = op_node;
    }
    
    return left_node;
}

Node* GetW(Node** nodes, int* cur_token)
{
    printf("I'm in getW\n");
    Node* left_node = GetP(nodes, cur_token);

    while (IS_SPEC_TYPE(nodes[*cur_token]) && IS_CMP(nodes[*cur_token]))
    {
        Node* op_node = nodes[*cur_token];
        (*cur_token)++;

        Node* right_node = GetP(nodes, cur_token);

        left_node->parent = op_node;
        op_node->left = left_node;
        right_node->parent = op_node;
        op_node->right = right_node;

        left_node = op_node;
    }
    return left_node;
}

Node* GetP(Node** nodes, int* cur_token)
{
    printf("I'm in getP\n");
    Node* new_node = NULL;

    if (IS_SPEC_TYPE(nodes[*cur_token]) && IS_BEGIN_COND(nodes[*cur_token]))
    {
        (*cur_token)++;
        new_node = GetE(nodes, cur_token);
        if (IS_SPEC_TYPE(nodes[*cur_token]) && !IS_END_COND(nodes[*cur_token])) Lexic_Error(nodes[*cur_token]->code_line);
        (*cur_token)++;
    }

    else if (IS_SPEC_TYPE(nodes[*cur_token]) && (IS_SIN(nodes[*cur_token]) || IS_COS(nodes[*cur_token]) || IS_POW(nodes[*cur_token]) || IS_CALL(nodes[*cur_token])))
    {
        switch (nodes[*cur_token]->value.l_type)
        {
        case SIN:
            new_node = nodes[*cur_token];
            break;

        case COS:
            new_node = nodes[*cur_token];
            break;
        case POW:
        {
            Node* new_node = nodes[*cur_token];
            (*cur_token)++;

            Get_Parametrs(nodes, cur_token, NULL, new_node);
            new_node->left->parent = new_node;

            (*cur_token)++;

            new_node->right = Create_Nil();
            new_node->right->parent = new_node;
            return new_node;
            break;
        }
        case CALL_FUNCTION:
        {
            Node* new_node = nodes[*cur_token];
            (*cur_token)++;

            Node* func_name = nodes[*cur_token];
            (*cur_token)++;
            func_name->parent = new_node;
            new_node->left = func_name;
            func_name->right = Create_Nil();
            func_name->right->parent = func_name;

            Get_Parametrs(nodes, cur_token, NULL, func_name);
            func_name->left->parent = func_name;

            (*cur_token)++;

            new_node->right = Create_Nil();
            new_node->right->parent = new_node;
            return new_node;
            break;
        }
        
        default:
            printf("??? ??, ????, ????? ?????????\n");
            break;
        }

        Node* argument_node = GetP(nodes, cur_token);

        argument_node->parent = new_node;
        new_node->left = argument_node;
        new_node->right = Create_Nil();
    }

    else
    {
        new_node = GetN(nodes, cur_token);
    }

    return new_node;
}

Node* GetN(Node** nodes, int* cur_token)
{
    printf("I'm in getP\n");

    Node* new_node = nodes[*cur_token];
    (*cur_token)++;

    return new_node;
}


//-------------------------------------Dump------------------------------------------------------------------------------------------------------------------------------

void Token_Dump(Node** nodes, int amount)
{
    FILE* Dump_File = Open_Dump(DUMP_TOKEN_FILE_NAME);

    Dump("digraph {\n");
    Dump("rankdir=LR;\n");
    Dump("node [ shape=record ];\n");

    for (int i = 0; i < amount; i++)
    {
        printf("sdfsdfsd     %d\n", nodes[i]->type);
        switch (nodes[i]->type)
        {
            case VALUE_TYPE:
                Dump("node_%d[label = \"type: VALUE | double: %lg | num = %d\", style = \"filled\", fillcolor = \"grey\"];\n", i, nodes[i]->num, nodes[i]->value.dval);
                break;
            
            case VAR_TYPE:
                Dump("node_%d[label = \"type: VARIABLE | name: %s | num = %d\", style = \"filled\", fillcolor = \"green\"];\n", i, nodes[i]->value.name, nodes[i]->num);
                break;

            case NAME_TYPE:
                Dump("node_%d[label = \"type: NAME F | name: %s | num = %d\", style = \"filled\", fillcolor = \"cyan\"];\n", i, nodes[i]->value.name, nodes[i]->num);
                break;

            case SPECIAL_TYPE:
                char* color = NULL;

                if (nodes[i]->value.l_type == START_EXP || nodes[i]->value.l_type == END_EXP || nodes[i]->value.l_type == END_AC) color = "yellow";
                else if (nodes[i]->value.l_type == FUNC_D || nodes[i]->value.l_type == FUNC_V) color = "blue";
                else color = "red";

                Dump("node_%d[label = \"type: LANG WORD | name: %s %s | num = %d\", style = \"rounded, filled\", fillcolor = \"%s\"];\n", i, Spec_Type_Name(nodes[i]->value.l_type), (nodes[i]->value.l_type == CALL_FUNCTION ? nodes[i]->call_func_name : " "), nodes[i]->num, color);
                break;
        }

        if (i == 0) continue;

        Dump("node_%d -> node_%d;\n", i-1, i);
    }

    Dump("}");
    fclose(Dump_File);

    char cmd[] = "dot -Tpng token.gv -o Token_Dump.png";
    system("iconv -f windows-1251 < token1.gv > token.gv");
    system("rm token1.gv");
    system(cmd);
}

const char* Spec_Type_Name(Language_Types type)
{
    switch (type)
    {
    case ADD:
        {
        return "+";
        break;
        }
    case SUB:
        {
        return "-";
        break;
        }
    case MUL:
        return "*";
        break;
    case DIV:
        return "/";
        break;
    case POW:
        return "^";
        break;
    case SIN:
        return "sin";
        break;
    case COS:
        return "cos";
        break;
    case SKIP:
        return "revalue";
        break;
    case EQ_EQ:
        return "equal";
        break;
    case LESS_EQ:
        return "less or eq";
        break;
    case MORE_EQ:
        return "more or eq";
        break;
    case LESS:
        return "less";
        break;
    case MORE:
        return "more";
        break;
    case NOT_EQ:
        return "not eq";
        break;
    case EQ:
        return "=";
        break;
    case VAR:
        return "var";
        break;
    case IF:
        return "if";
        break;
    case ELSE:
        return "else";
        break;
    case WHILE:
        return "while";
        break;
    case START_EXP:
        return "start exp";
        break;
    case END_EXP:
        return "end exp";
        break;
    case BEGIN_COND:
        return "begin cond";
        break;
    case END_COND:
        return "end cond";
        break;
    case END_AC:
        return ";";
        break;
    case FUNC_D:
        return "type function";
        break;
    case FUNC_V:
        return "void function";
        break;
    case CALL_FUNCTION:
        return "call";
        break;
    case RET:
        return "return";
        break;
    case SCAN:
        return "scanf";
        break;
    case OUT:
        return "print";
        break;
    case AND:
        return "and";
        break;
    case OR:
        return "or";
        break;
    case NIL:
        return "NIL";
        break;
    case ST:
        return "Statement";
        break;
    case PARAM:
        return "Param";
        break;
    case VOID:
        return "Void";
        break;
    case TYPE:
        return "Type";
        break;
    }
}

const char* Spec_Type_Name_Standart(Language_Types type)
{
    switch (type)
    {
    case ADD:
        {
        return "ADD";
        break;
        }
    case SUB:
        {
        return "SUB";
        break;
        }
    case MUL:
        return "MUL";
        break;
    case DIV:
        return "DIV";
        break;
    case POW:
        return "POW";
        break;
    case SIN:
        return "SIN";
        break;
    case COS:
        return "COS";
        break;
    case SKIP:
        return "EQ";
        break;
    case EQ_EQ:
        return "IS_EE";
        break;
    case LESS_EQ:
        return "IS_BE";
        break;
    case MORE_EQ:
        return "IS_GE";
        break;
    case LESS:
        return "IS_BT";
        break;
    case MORE:
        return "IS_GT";
        break;
    case NOT_EQ:
        return "IS_NE";
        break;
    case EQ:
        return "=";
        break;
    case VAR:
        return "VAR";
        break;
    case IF:
        return "IF";
        break;
    case ELSE:
        return "ELSE";
        break;
    case WHILE:
        return "WHILE";
        break;
    case START_EXP:
        return "start exp";
        break;
    case END_EXP:
        return "end exp";
        break;
    case BEGIN_COND:
        return "begin cond";
        break;
    case END_COND:
        return "end cond";
        break;
    case END_AC:
        return ";";
        break;
    case FUNC_D:
        return "FUNC";
        break;
    case FUNC_V:
        return "FUNC";
        break;
    case CALL_FUNCTION:
        return "CALL";
        break;
    case RET:
        return "RET";
        break;
    case SCAN:
        return "IN";
        break;
    case OUT:
        return "OUT";
        break;
    case AND:
        return "AND";
        break;
    case OR:
        return "OR";
        break;
    case NIL:
        return "NIL";
        break;
    case ST:
        return "ST";
        break;
    case PARAM:
        return "PARAM";
        break;
    case VOID:
        return "VOID";
        break;
    case TYPE:
        return "TYPE";
        break;
    }
}

void Proga_Dump()
{
    FILE* Dump_File = Open_Dump(DUMP_PROGA_FILE_NAME);

    Dump("digraph {\n");
    //Dump("rankdir=LR;\n");
    Dump("nodesep = 2;\n");
    Dump("rankdir=TB;\n");
    Dump("node [ shape=record, style = rounded, fontsize = 30];\n");

    Recursive_Node_Dump(Main_Node, Dump_File);

    Dump("}");
    fclose(Dump_File);

    char cmd[] = "dot -Tpng proga.gv -o Proga_Dump.png";
    system("iconv -f windows-1251 < proga1.gv > proga.gv");
    system("rm proga1.gv");
    system(cmd);

}

void Recursive_Node_Dump(const Node* node, FILE* Dump_File)
{
    switch (node->type)
    {
        case VALUE_TYPE:
            Dump("node_%d[label = \"{type: VALUE | double: %lg}\", style = \"filled\", fillcolor = \"grey\"];\n", node->num, node->value.dval);
            break;
            
        case VAR_TYPE:
            Dump("node_%d[label = \"{type: VARIABLE | name: %s}\", style = \"filled\", fillcolor = \"green\"];\n", node->num, node->value.name);
            break;

        case NAME_TYPE:
            Dump("node_%d[label = \"{type: NAME F | name: %s}\", style = \"filled\", fillcolor = \"cyan\"];\n", node->num, node->value.name);
            break;

        case SPECIAL_TYPE:
            char* color = NULL;

            if (node->value.l_type == PARAM) color = "hotpink2";
            else if (node->value.l_type == ST) color = "yellow";
            else if (node->value.l_type == FUNC_D || node->value.l_type == FUNC_V) color = "red";
            else if (node->value.l_type == VOID) color = "moccasin";
            else if (node->value.l_type == TYPE) color = "orange";
            else if (node->value.l_type == NIL) color = "blue1";
            else color = "pink";

            Dump("node_%d[label = \"{type: LANG WORD | name: %s %s}\", style = \"rounded, filled\", fillcolor = \"%s\"];\n", node->num, Spec_Type_Name(node->value.l_type), (node->value.l_type == CALL_FUNCTION ? node->call_func_name : " "), color);
            break;
    }

    if (node->left != NULL)
    {
        Recursive_Node_Dump(node->left, Dump_File);
        Dump("node_%d->node_%d [color = \"darkgreen\"]\n", node->num, node->left->num);
    }

    if (node->right != NULL)
    {
        Recursive_Node_Dump(node->right, Dump_File);
        Dump("node_%d->node_%d [color = \"darkred\"]\n", node->num, node->right->num);
    }
}


void Standart_Dump()
{
    FILE* Dump_File = fopen("../middlend/code.txt", "w");
    Standart_Recursion(Dump_File, 0, Main_Node);
}

void Standart_Recursion(FILE* Dump_File, int tabs, Node* node)
{
    printf("i'm gegegegegegegegegegg, num is %d\n", node->num);
    Dump("\n");
    for (int i = 0; i < tabs; i++) Dump("\t");

    Dump("{ ");

    if (IS_SPEC_TYPE(node)) {Dump("%s", Spec_Type_Name_Standart(node->value.l_type));}
    else if (IS_NAME(node) || IS_VAR(node)) {Dump("\"%s\"", node->value.name);}
    else {Dump("%lg", node->value.dval);}

    Dump(" ");

    bool has_child = false;

    if(node->left != NULL) Standart_Recursion(Dump_File, tabs+1, node->left);
    if(node->right != NULL) Standart_Recursion(Dump_File, tabs+1, node->right);

    Dump("} ");
}