#include "include/headers/dump.h"

FILE* Open_Dump(const char* name) 
{
    FILE* Dump = fopen(name, "w");
    if (Dump == NULL) printf("ERROR, CAN'T OPEN DUMP FILE\n");
    return Dump;
}

void Token_Dump(Node** nodes, int amount)
{
    FILE* Dump_File = Open_Dump(DUMP_TOKEN_FILE_NAME);

    Dump("digraph {\n");
    Dump("rankdir=LR;\n");
    Dump("node [ shape=record ];\n");

    for (int i = 0; i < amount; i++)
    {
        //printf("sdfsdfsd     %d\n", nodes[i]->type);
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

    char cmd[] = "dot -Tpng dump/token.gv -o dump/Token_Dump.png";
    system("iconv -f windows-1251 < dump/token1.gv > dump/token.gv");
    system("rm dump/token1.gv");
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
    case SQRT:
        return "sqrt";
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
    case SQRT:
        return "SQRT";
        break;
    }
}

void Proga_Dump(Node* Main_Node)
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

    char cmd[] = "dot -Tpng dump/proga.gv -o dump/Proga_Dump.png";
    system("iconv -f windows-1251 < dump/proga1.gv > dump/proga.gv");
    system("rm dump/proga1.gv");
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


void Standart_Dump(const int argc, const char* argv[], Node* Main_Node)
{
    FILE* Dump_File = NULL;
    if (argc < 3)
    {
        Dump_File = fopen("../middlend/standard_code/code.txt", "w");
        if (Dump_File == NULL)
        {
            printf("Impossible to open out file with default name\n");
            exit(666);
        }
    }
    else
    {
        char name[70] = "../middlend/standard_code/";
        strcat(name, argv[2]);
        Dump_File = fopen(name, "w");
        if (Dump_File == NULL)
        {
            printf("Impossible to open out file with given name\n");
            exit(666);
        }
    }
    
    Standart_Recursion(Dump_File, 0, Main_Node);
}

void Standart_Recursion(FILE* Dump_File, int tabs, Node* node)
{
    //printf("i'm gegegegegegegegegegg, num is %d\n", node->num);
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