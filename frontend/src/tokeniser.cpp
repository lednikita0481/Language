#include "include/headers/tokeniser.h"
#include "include/headers/DSL.h"
#include "include/headers/dump.h"
#include "include/headers/dictionary.h"
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
    if ((p <= 'ÿ' && p >= 'à') || (p <= 'ß' && p >= 'À') || isalnum(p) || p == '_') return 1;
    else return 0;
}

char* Get_Lim_Len_String(char* to, const char* from, const int lendth)
{
    for (int i = 0; i < lendth; i++)
    {
        to[i] = from[i];
    }

    //printf("name is %s\n", to);
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

int Tokenise(Node** nodes, Text* text)
{
    int current_line = 1;
    int current_token = 0;
    int pointer = 0;

    while (pointer < text->symbols)
    {
        //printf("Pointer is %d, line is %d %10s\n", pointer, current_line, text->buff+pointer);
        bool found_node = false;

        for (int i = 0; i < COMMANDS_NUMBER; i++)
        {
            if (strncmp(text->buff+pointer, COMMANDS[i].name, COMMANDS[i].lendth) == 0)
            {
                found_node = true;
                //printf("hehe I found node!\n");
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
                //printf("name len is %d\n\n\n", name_len);
                char* name = (char*) calloc(name_len+1, sizeof(char));
                Get_Lim_Len_String(name, (text->buff + pointer), name_len);

                Syntax_Assert(!isdigit(name[0]));

                nodes[current_token] = Create_Var(name, current_line);

                current_token++;
                pointer += name_len;

                SSaC();
            }

            /*else if (nodes[current_token-1]->value.l_type == EQ)
            {
                double val = Get_Num(text->buff + pointer, &pointer);
                nodes[current_token] = Create_Value(val, current_line);

                current_token++;
                SSaC();
            }*/

            else if (nodes[current_token-1]->value.l_type == FUNC_D || nodes[current_token-1]->value.l_type == FUNC_V)
            {
                int name_len = Next_Word_Len(text, &pointer);
                //printf("len is %d\n", name_len);
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

                Syntax_Assert(name_len != 0);

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
    //printf("%d\n", current_token);
    return current_token;
}