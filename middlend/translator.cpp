#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#include "include/translator.h"
#include "include/DSL.h"

int main(const int argc, const char* argv[])
{
    char* text = NULL;
    Open_Read_File(argc, argv, text);
    MAIN_NODE = Scan_Node();
    Proga_Dump();
    Close_Read_File();
    Open_Write_File(argc, argv);
    Start_Asm_Dump();
    Close_Write_File();
}

//-------------------------------------HELP FUNCTIONS---------------------------------------------------------------------------------------------------------------

void Open_Read_File(const int argc, const char* argv[], char* text)
{
    if (argc == 1)
    {
        CODE_FILE = fopen(DEFAULT_CODE_FILE_NAME, "r");
        printf("opened file!\n");

        if (CODE_FILE == NULL)
        {
            printf("Impossible to open code file with default name, create it or write file name as second argument\n");
            exit(666);
        }
    }

    else if (argc >= 2)
    {
        CODE_FILE = fopen(argv[1], "r");

        if (CODE_FILE == NULL)
        {
            printf("Impossible to open code file with this name\n");
            exit(666);
        }
    }

    else
    {
        printf("Write the second argument if you needs to open certain file or no arguments if you want to use defaul settings\n");
        exit(666);
    }

    /*
    struct stat buff = {};
    fstat(fileno(CODE_FILE), &buff);
    int size = buff.st_size;

    text = (char*) calloc(size, sizeof(char));

    fread(text, sizeof(char), size, CODE_FILE);

    fclose(CODE_FILE);
    */
}

void Close_Read_File()
{
    fclose(CODE_FILE);
}

void Open_Write_File(const int argc, const char* argv[])
{
    char file_path[100] = {};
    if (argc < 3)
    {
        sprintf(file_path, "%s%s", "../backend/", DEFAULT_ASM_FILE_NAME);
        ASM_FILE = fopen(file_path, "w");
    }

    else 
    {
        sprintf(file_path, "%s%s", "../backend/", argv[2]);
        ASM_FILE = fopen(file_path, "w");
    }
}

void Close_Write_File()
{
    fclose(CODE_FILE);
}

Node* Create_Node(Types type, double dval, Language_Types l_type, char* name, char* call_func_name, Node* left, Node* right, Node* parent)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));

    new_node->type = type;
    new_node->left = left;
    new_node->right = right;
    new_node->parent = parent;
    new_node->call_func_name = call_func_name;
    new_node->num = N_NUM;
    N_NUM++;

    if      (type == VALUE_TYPE)    new_node->value.dval = dval;
    else if (type == VAR_TYPE)      new_node->value.name = name;
    else if (type == SPECIAL_TYPE)  new_node->value.l_type = l_type;
    else if (type == NAME_TYPE)     
    {
        new_node->value.name = name;
    }

    if (type == SPECIAL_TYPE && type == CALL) new_node->call_func_name = call_func_name;

    return new_node;
}

//----------------------------------------------Tree Ctor-----------------------------------------------------------------------------------------------------


Node* Scan_Node()
{
    Node* new_node = NULL;
    bool has_children = false;

    char line[MAX_LEN_ELEM] = {};

    int dfoihjro = 0;

    fscanf(CODE_FILE, " { %s ", line);
    //printf("%d\n", dfoihjro);
    printf("Here is scan node, line is %s\n", line);

    int terminates = 0;
    fscanf(CODE_FILE, " }%n", &terminates);
    if (terminates == 0) has_children = true;
    printf("%d has child\n", has_children);

    Language_Types op = Chech_L_Type(line);

    if (op == NOT_LANG_TYPE)
    {
        double val = 0;
        
        if (sscanf(line, "%lg", &val) != 0)
        {
            new_node = Create_Value(val);
        }

        else
        {
            char val[MAX_LEN_ELEM] = {};

            if (sscanf(line, "\"%[^\"]\"", val) == 0)
            {
                printf("You are maloletniy debil. Rebuild your standart file!\n");
                exit(3453464375673635789573925);
            }

            char* name = (char*) calloc(strlen(val)+1, sizeof(char));
            strcpy(name, val);

            new_node = Create_Var(name);
        }
    }
    else
    {
        new_node = Create_Spec(op);
    }

    if (!has_children) return new_node;

    new_node->left = Scan_Node();
    new_node->left->parent = new_node;
    new_node->right = Scan_Node();
    new_node->right->parent = new_node;

    if (op == FUNC)
    {
        new_node->left->type = NAME_TYPE;
    }

    fscanf(CODE_FILE, " } %n", &terminates);

    return new_node;
}

Language_Types Chech_L_Type(char* line)
{
    for (int i = 0; i < COMMAND_AMOUNT; i++)
    {
        if(strncmp(line, COMMANDS[i].name, COMMANDS[i].lendth) == 0) return COMMANDS[i].l_type;
    }

    return NOT_LANG_TYPE;
}

//------------------------------------------------------TO ASM-----------------------------------------------------------------------------------------

int Get_Var(char* var)
{
    printf("GETVAR!!! node char* is %s\n", var);
    for (int i = 0; i < CUR_VAR; i++)
    {
        printf("i'm in this shit, cur var is %s\n", VARIABLES[i]);
        if (strcmp(var, VARIABLES[i]) == 0)
        {
            return i;
        }
    }

    VARIABLES[CUR_VAR] = var;
    //VARIABLES[CUR_VAR] = (char*) calloc(strlen(var) + 1, sizeof(char));
    //strcpy(VARIABLES[CUR_VAR], node->value.name);
    CUR_VAR++;
    printf("ENDGETVAR!\n");
    return (CUR_VAR-1);
}

void Start_Asm_Dump()
{
    PRINT_ASM("call main");
    PRINT_ASM("hlt");
    PRINT_ASM("\n");
    Asm_Node(MAIN_NODE);
}

void Asm_Node(Node* node, Language_Types previous_node_type)
{  
    printf("I'm here! pointer is %p\n", node);
    printf("node num is %d", node->num);
    printf("node type is %d and num is %d\n", node->type, node->num);
    switch (node->type)
    {
        case SPECIAL_TYPE:
        {
            printf("special type is %d\n", node->value.l_type);
            switch (node->value.l_type)
            {
                case ST:
                    Asm_Node(LEFT_C);
                    if (!IS_NIL(RIGHT_C)) Asm_Node(RIGHT_C);
                    break;
                case FUNC:
                    Asm_Node(LEFT_C);
                    Asm_Node(RIGHT_C);
                    break;
                case RET:
                    if (!IS_NIL(LEFT_C)) Asm_Node(LEFT_C);
                    PRINT_ASM("RET");
                case PARAM:
                    if (previous_node_type == SCAN)
                    {
                        Node* cur_node = node;
                        while (!IS_NIL(cur_node->right));
                        {
                            PRINT_ASM("in");
                            PRINT_ASM("pop[%d]", Get_Var(cur_node->left->value.name));
                            cur_node = cur_node->right;
                        }
                        PRINT_ASM("in");
                        PRINT_ASM("pop[%d]", Get_Var(cur_node->left->value.name));
                    }

                    else if (previous_node_type == OUT)
                    {
                        Node* cur_node = node;
                        while (!IS_NIL(cur_node->right));
                        {
                            Asm_Node(cur_node->left);
                            PRINT_ASM("out");
                            cur_node = cur_node->right;
                        }
                        Asm_Node(cur_node->left);
                        PRINT_ASM("out");
                    }

                    else
                    {
                        Node* cur_node = node;
                        while (!IS_NIL(cur_node->right));
                        {
                            PRINT_ASM("push[%d]", Get_Var(cur_node->left->value.name));
                            cur_node = cur_node->right;
                        }
                        PRINT_ASM("push[%d]", Get_Var(cur_node->left->value.name));
                    }
                    break;

                case IF:
                    if (node->right->value.l_type == ST)
                    {
                        int a = IF_LABELS;
                        IF_LABELS++;
                        int b = IF_LABELS;
                        Asm_Condition(node->left, false, a, b);
                        PRINT_ASM("if_label_%d:", a);
                        Asm_Node(RIGHT_C);
                        PRINT_ASM("if_label_%d:", b);
                    }
                    else
                    {
                        int a = IF_LABELS;
                        IF_LABELS++;
                        int b = IF_LABELS;
                        IF_LABELS++;
                        int c = IF_LABELS;
                        IF_LABELS++;
                        Asm_Condition(node->left, false, a, b);
                        PRINT_ASM("if_label_%d:", a);
                        Asm_Node(node->right->left);
                        PRINT_ASM("jmp if_label_%d", c);
                        PRINT_ASM("if_label_%d:", b);
                        Asm_Node(node->right->right);
                        PRINT_ASM("if_label_%d:", c);
                    }
                    break;
                    
                case WHILE:
                {
                    int a = IF_LABELS;
                    IF_LABELS++;
                    int b = IF_LABELS;
                    IF_LABELS++;

                    PRINT_ASM("if_label_%d:", a);
                    Asm_Condition(node->left, true, a, b);
                    Asm_Node(node->right);
                    PRINT_ASM("jmp if_label_%d", a);
                    PRINT_ASM("if_label_%d:", b);
                    break;
                }

                case ADD:
                {
                    Asm_Node(node->left);
                    Asm_Node(node->right);
                    PRINT_ASM("add");
                    break;
                }
                case SUB:
                {
                    Asm_Node(node->left);
                    Asm_Node(node->right);
                    PRINT_ASM("sub");
                    break;
                }
                case MUL:
                {
                    Asm_Node(node->left);
                    Asm_Node(node->right);
                    PRINT_ASM("mul");
                    break;
                }
                case DIV:
                {
                    Asm_Node(node->left);
                    Asm_Node(node->right);
                    PRINT_ASM("div");
                    break;
                }
                case SIN:
                {
                    Asm_Node(node->left);
                    PRINT_ASM("sin");
                    break;
                }
                case COS:
                {
                    Asm_Node(node->left);
                    PRINT_ASM("cos");
                    break;
                }
                case POW:
                {
                    Asm_Node(node->left);
                    Asm_Node(node->right);
                    PRINT_ASM("pow");
                    break;
                }

                case EQ:
                {
                    Asm_Node(node->right);
                    PRINT_ASM("pop[%d]", Get_Var(node->left->value.name));
                    break;
                }

                case VAR:
                {
                    Asm_Node(node->right);
                    PRINT_ASM("pop[%d]", Get_Var(node->left->value.name));
                    break;
                }

                case SCAN:
                {
                    Asm_Node(node->left, SCAN);
                    break;
                }
                case OUT:
                {
                    Asm_Node(node->left, OUT);
                    break;
                }
                default:
                    printf("we are in shit!\n");
                    break;
                        
            }
            break;
        }

        case VAR_TYPE:
        {
            printf("var is %s\n", node->value.name);
            PRINT_ASM("push[%d]", Get_Var(node->value.name));
            printf("var_fdnhjooblfdhjjkrethnrt\n");
            break;
        }

        case NAME_TYPE:
        {
            printf("name is %s\n", node->value.name);
            PRINT_ASM("%s", node->value.name);
            break;
        }
        case VALUE_TYPE:
        {
            printf("value is %lf\n", node->value.dval);
            PRINT_ASM("%lf", node->value.dval);
            break;
        }
        
        default:
            printf("wtf man??\n");
            break;
    }

    printf("\n\n");
}

void Asm_Condition(Node* node, bool reversed, int begin_if, int begin_else)
{
    printf("Asm CONDITION!\n");
    switch (node->value.l_type)
    {
    case OR:
    {
        printf("or\n");
        int end_or = IF_LABELS;
        IF_LABELS++;
        Asm_Condition(node->left, reversed, end_or, begin_else);
        Asm_Condition(node->right, reversed, end_or, begin_else);
        PRINT_ASM("jmp if_label_%d", begin_else);
        PRINT_ASM("if_label_%d:", end_or);
        PRINT_ASM("jmp if_label_%d", begin_if);
        break;
    }

    case AND:
    {
        printf("and\n");
        int end_and = IF_LABELS;
        IF_LABELS++;
        Asm_Condition(node->left, !reversed, begin_if, end_and);
        Asm_Condition(node->right, !reversed, begin_if, end_and);
        PRINT_ASM("jmp if_label_%d", begin_if);
        PRINT_ASM("if_label_%d:", end_and);
        PRINT_ASM("jmp if_label_%d", begin_else);
    }

    case IS_EE:
    {
        printf("is ee\n");
        Asm_Node(node->left);
        Asm_Node(node->right);
        if (!reversed)
        {
            PRINT_ASM("je if_label_%d", begin_if);
        }
        else
        {
            PRINT_ASM("jne id_label_%d", begin_else);
        }
        break;
    }
    
    case IS_BE:
    {
        printf("is be\n");
        Asm_Node(node->left);
        Asm_Node(node->right);
        if (!reversed) 
        {
            PRINT_ASM("jbe if_label_%d", begin_if);
        }
        else 
        {
            PRINT_ASM("ja if_label_%d", begin_else);
        }
        break;
    }
    
    case IS_GE:
    {
        printf("is ge\n");
        Asm_Node(node->left);
        Asm_Node(node->right);
        if (!reversed)
        {
            PRINT_ASM("jae if_label_%d", begin_if);
        }
        else
        {
            PRINT_ASM("jb if_label_%d", begin_else);
        }
        break;
    }

    case IS_BT:
    {
        printf("is bt\n");
        Asm_Node(node->left);
        Asm_Node(node->right);
        if (!reversed)
        {
            PRINT_ASM("jb if_label_%d", begin_if);
        }
        else
        {
            PRINT_ASM("jae if_label_%d", begin_else);
        }
        break;
    }

    case IS_GT:
    {
        printf("is gt\n");
        Asm_Node(node->left);
        Asm_Node(node->right);
        if (!reversed)
        {
            PRINT_ASM("ja if_label_%d", begin_if);
        }
        else
        {
            PRINT_ASM("jbe if_label_%d", begin_else);
        }
        break;
    }

    case IS_NE:
    {
        printf("is ne\n");
        Asm_Node(node->left);
        Asm_Node(node->right);
        if (!reversed)
        {
            PRINT_ASM("jne if_label_%d", begin_if);
        }
        else
        {
            PRINT_ASM("je if_label_%d", begin_else);
        }
        break;
    }
    
    default:
        break;
    }
}


//---------------------------------------------------------DUMP----------------------------------------------------------------------------------------

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
    case EQ:
        return "revalue";
        break;
    case IS_EE:
        return "equal";
        break;
    case IS_BE:
        return "less or eq";
        break;
    case IS_GE:
        return "more or eq";
        break;
    case IS_BT:
        return "less";
        break;
    case IS_GT:
        return "more";
        break;
    case IS_NE:
        return "not eq";
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
    case FUNC:
        return "function";
        break;
    case CALL:
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

void Proga_Dump()
{
    FILE* Dump_File = Open_Dump(DUMP_PROGA_FILE_NAME);

    Dump("digraph {\n");
    //Dump("rankdir=LR;\n");
    Dump("nodesep = 2;\n");
    Dump("rankdir=TB;\n");
    Dump("node [ shape=record, style = rounded, fontsize = 30];\n");

    Recursive_Node_Dump(MAIN_NODE, Dump_File);

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
            Dump("node_%d[label = \"{type: VALUE | double: %lg | num: %d}\", style = \"filled\", fillcolor = \"grey\"];\n", node->num, node->value.dval, node->num);
            break;
            
        case VAR_TYPE:
            Dump("node_%d[label = \"{type: VARIABLE | name: %s | num: %d}\", style = \"filled\", fillcolor = \"green\"];\n", node->num, node->value.name, node->num);
            break;

        case NAME_TYPE:
            Dump("node_%d[label = \"{type: NAME F | name: %s | num: %d}\", style = \"filled\", fillcolor = \"cyan\"];\n", node->num, node->value.name, node->num);
            break;

        case SPECIAL_TYPE:
            char* color = NULL;

            if (node->value.l_type == PARAM) color = "hotpink2";
            else if (node->value.l_type == ST) color = "yellow";
            else if (node->value.l_type == FUNC) color = "red";
            else if (node->value.l_type == VOID) color = "moccasin";
            else if (node->value.l_type == TYPE) color = "orange";
            else if (node->value.l_type == NIL) color = "blue1";
            else color = "pink";

            Dump("node_%d[label = \"{type: LANG WORD | name: %s %s | num: %d}\", style = \"rounded, filled\", fillcolor = \"%s\"];\n", node->num, Spec_Type_Name(node->value.l_type), (node->value.l_type == CALL ? node->call_func_name : " "), node->num, color);
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
