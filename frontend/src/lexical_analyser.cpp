#include "include/headers/lexical_analyser.h"

Function FUNCTIONS[100] = {};
int F_NUM = 0;

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

void Lexic_Error(int line)
{
    printf("Lexical error in line %d!\n", line);
    exit(666);
}

Node* Lexical_Tree_Connector(Node** nodes, int Tokens_Amount)
{
    int cur_token = 0;

    while(cur_token < Tokens_Amount-1)
    {


        //Хочу кринжа и делаю!


        //printf("node num is %d\n", nodes[cur_token]->num);
        switch (nodes[cur_token]->type)
        {
            case SPECIAL_TYPE:
                switch (nodes[cur_token]->value.l_type)
                {
                    case FUNC_D:
                    {
                        Node* func_node = nodes[cur_token];
                        //printf("I'm in function node, node is %d\n", func_node->num);
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
                        //printf("I'm in function node, node is %d\n", func_node->num);
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
    Node* Main_Node = Create_St();
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

    return Main_Node;
}

//function creation processing __--__--__--__--__--__--__--__--__--__--

void Get_Func_Name(Node** nodes, int* cur_token, Node* func_node)
{
    Node* name_node = nodes[*cur_token];
    //printf("I'm in get func name node, num is %d\n", name_node->num);

    if (name_node->type != NAME_TYPE) Lexic_Error(name_node->code_line);

    name_node->parent = func_node;
    func_node->left = name_node;
    (*cur_token)++;
    //printf("lndflgndfgdnrg\n");
    Get_Parametrs(nodes, cur_token, func_node, name_node);
}

void Get_Parametrs(Node** nodes, int* cur_token, Node* func_node, Node* name_node)
{
    //printf("hrhrhrhrh current token %d\n", *cur_token);
    //printf("%d\n", nodes[*cur_token]->type != SPECIAL_TYPE);
    //printf("%d\n", nodes[*cur_token]->value.l_type != BEGIN_COND);
    if (nodes[*cur_token]->type != SPECIAL_TYPE || nodes[*cur_token]->value.l_type != BEGIN_COND) Lexic_Error(nodes[*cur_token]->code_line);

    (*cur_token)++;

    //printf("I'm in get parametrs\n");

    if (nodes[*cur_token]->type == SPECIAL_TYPE && nodes[*cur_token]->value.l_type == END_COND)
    {
        Node* nil = Create_Nil();
        nil->parent = name_node;
        name_node->left = nil;
    }

    else name_node->left =  Get_Param(nodes, cur_token, func_node);
    name_node->left->parent = name_node;

    //printf("I'm in get parametrs again\n");
}

Node* Get_Param(Node** nodes, int* cur_token, Node* func_node)
{

    Node* param_node = Create_Param();
    Node* param_value_node = nodes[*cur_token];
    //printf("I'm in get param node, line is %d\n", param_value_node->code_line);
    param_node->left = param_value_node;
    param_value_node->parent = param_node;

    (*cur_token)++;

    Node* next_node = nodes[*cur_token];
    if (nodes[*cur_token]->type == SPECIAL_TYPE && nodes[*cur_token]->value.l_type == END_COND)
    {
        //printf("I'm in get param node end cond, num is %d\n", nodes[*cur_token]->num);
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
    //printf("I'm in get statement exp node, num is %d\n", begin_node->num);

    if (begin_node->type != SPECIAL_TYPE || begin_node->value.l_type != START_EXP) 
    {
        Lexic_Error(begin_node->code_line);
    }
    (*cur_token)++;

    Node* return_node = Get_Statement(nodes, cur_token);

    begin_node = nodes[*cur_token];
    if (begin_node->type != SPECIAL_TYPE || begin_node->value.l_type != END_EXP) Lexic_Error(begin_node->code_line);
    //printf("here!!!\n");

    return return_node;
}

Node* Get_Statement(Node** nodes, int* cur_token)
{
    Node* st_node = Create_St();

    //printf("I'm in get statement node\n");

    st_node->left = Get_Action(nodes, cur_token);
    st_node->parent = st_node;
    Node* node = nodes[*cur_token];
    //printf("I'm in get statement agian, num is %d\n", node->num);
    
    if (node->type == SPECIAL_TYPE && node->value.l_type == END_EXP)
    {
        //printf("alalalala\n");
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
    //printf("I'm in get action node, num is %d\n", see_node->num);
    //printf("%d\n", IS_SPEC_TYPE(see_node));
    bool check = IS_SPEC_TYPE(see_node);
    if (!check)
    {
        if (NEEDED_NODE(see_node)) check = true;
    }

    if (check)
    {
        switch (see_node->value.l_type)
        {
        case VAR:
        {
            //printf("it's var node\n");
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
            //printf("it's if node\n");
            Node* if_node = see_node;
            (*cur_token)++;

            see_node = nodes[*cur_token];
            if (!(IS_SPEC_TYPE(see_node)) || !IS_BEGIN_COND(see_node)) Lexic_Error(see_node->code_line);

            (*cur_token)++;
            if_node->left = GetE(nodes, cur_token);
            if_node->left->parent = if_node;

            //printf("herere current token is %d\n", *cur_token);
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
                //printf("it's else node\n");
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
            //printf("it's while node\n");
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
            //printf("it's call function node\n");
            Node* call_node = see_node;
            //printf("call node num is %d\n", call_node->num);
            (*cur_token)++;
            Node* func_name = nodes[*cur_token];
            //printf("func name node num is %d\n", func_name->num);
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
            //printf("it's ret node\n");
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
            //printf("it's var revalue node\n");
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
            //printf("it's scan node\n");
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
            //printf("it's out node\n");
            Node* scan_node = see_node;
            (*cur_token)++;

            Get_Parametrs(nodes, cur_token, NULL, scan_node);
            (*cur_token)++;
            //printf("get back from get param, cur token is %d\n", *cur_token);
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

    else
    {
        Lexic_Error(see_node->code_line);
    }
}



//Numbers and algebra --__---__--__-___-__--__--_---__--__--__--_--

Node* GetE(Node** nodes, int* cur_token)
{
    //printf("I'm in getE\n");
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
    //printf("I'm in getW\n");
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
    //printf("I'm in getT\n");
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
    //printf("I'm in getW\n");
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
    //printf("I'm in getP\n");
    Node* new_node = NULL;

    if (IS_SPEC_TYPE(nodes[*cur_token]) && IS_BEGIN_COND(nodes[*cur_token]))
    {
        (*cur_token)++;
        new_node = GetE(nodes, cur_token);
        if (IS_SPEC_TYPE(nodes[*cur_token]) && !IS_END_COND(nodes[*cur_token])) Lexic_Error(nodes[*cur_token]->code_line);
        (*cur_token)++;
    }

    else if (IS_SPEC_TYPE(nodes[*cur_token]) && (IS_SIN(nodes[*cur_token]) || IS_COS(nodes[*cur_token]) || IS_POW(nodes[*cur_token]) || IS_CALL(nodes[*cur_token]) || IS_SQRT(nodes[*cur_token])))
    {
        switch (nodes[*cur_token]->value.l_type)
        {
        case SIN:
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

        case COS:
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
        case SQRT:
        {
            Node* new_node = nodes[*cur_token];
            (*cur_token)++;

            Get_Parametrs(nodes, cur_token, NULL, new_node);
            //new_node->left = GetE(nodes, cur_token);
            new_node->left->parent = new_node;

            (*cur_token)++;

            new_node->right = Create_Nil();
            new_node->right->parent = new_node;
            return new_node;
            break;
        }
        case POW:
        {
            Node* new_node = nodes[*cur_token];
            (*cur_token)++;

            //Get_Parametrs(nodes, cur_token, NULL, new_node);
            new_node->left = GetE(nodes, cur_token);
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
    //printf("I'm in getP\n");

    Node* new_node = nodes[*cur_token];
    (*cur_token)++;

    return new_node;
}