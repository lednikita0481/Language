#define Create_Value(dval)              Create_Node(VALUE_TYPE, dval, NOT_LANG_TYPE, NULL, NULL, NULL, NULL, NULL)
#define Create_Var(name)                Create_Node(VAR_TYPE, 0, NOT_LANG_TYPE, name, NULL, NULL, NULL, NULL)
#define Create_Spec(l_type)             Create_Node(SPECIAL_TYPE, 0, l_type, NULL, NULL, NULL, NULL, NULL)
#define Create_Name(name)               Create_Node(NAME_TYPE, 0, NOT_LANG_TYPE, name, NULL, NULL, NULL, NULL)
#define Create_Nil()                    Create_Node(SPECIAL_TYPE, 0, NIL, NULL, NULL, NULL, NULL, NULL)
#define Create_Param()                  Create_Node(SPECIAL_TYPE, 0, PARAM, NULL, NULL, NULL, NULL, NULL)
#define Create_St()                     Create_Node(SPECIAL_TYPE, 0, ST, NULL, NULL, NULL, NULL, NULL)
#define Create_Void()                   Create_Node(SPECIAL_TYPE, 0, VOID, NULL, NULL, NULL, NULL, NULL)
#define Create_Type()                   Create_Node(SPECIAL_TYPE, 0, TYPE, NULL, NULL, NULL, NULL, NULL)

#define IS_NIL(node)                    (node->type == SPECIAL_TYPE && node->value.l_type == NIL)

#define LEFT_C                          node->left
#define RIGHT_C                         node->right