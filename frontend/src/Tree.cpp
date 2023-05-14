#include "include/headers/Language.h"
#include "include/headers/DSL.h"
#include "include/headers/Tree.h"

int IMAGE_NUM = 0;

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