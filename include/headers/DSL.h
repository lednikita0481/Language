#define Create_Value(dval, code_line)   Create_Node(VALUE_TYPE, dval, NOT_LANG_TYPE, NULL, NULL, NULL, NULL, NULL, code_line)
#define Create_Var(name, code_line)     Create_Node(VAR_TYPE, 0, NOT_LANG_TYPE, name, NULL, NULL, NULL, NULL, code_line)
#define Create_Spec(l_type, code_line)  Create_Node(SPECIAL_TYPE, 0, l_type, NULL, NULL, NULL, NULL, NULL, code_line)
#define Create_Name(name, code_line)    Create_Node(NAME_TYPE, 0, NOT_LANG_TYPE, name, NULL, NULL, NULL, NULL, code_line)
#define Create_Nil()                    Create_Node(SPECIAL_TYPE, 0, NIL, NULL, NULL, NULL, NULL, NULL, -1)
#define Create_Param()                  Create_Node(SPECIAL_TYPE, 0, PARAM, NULL, NULL, NULL, NULL, NULL, -1)
#define Create_St()                     Create_Node(SPECIAL_TYPE, 0, ST, NULL, NULL, NULL, NULL, NULL, -1)
#define Create_Void()                   Create_Node(SPECIAL_TYPE, 0, VOID, NULL, NULL, NULL, NULL, NULL, -1)
#define Create_Type()                   Create_Node(SPECIAL_TYPE, 0, TYPE, NULL, NULL, NULL, NULL, NULL, -1)

#define IS_SPEC_TYPE(node)              node->type == SPECIAL_TYPE
#define IS_NAME(node)                   node->type == NAME_TYPE
#define IS_NAME(node)                   node->type == VAR_TYPE
#define IS_EQ(node)                     node->value.l_type == EQ
#define IS_ADD(node)                    node->value.l_type == ADD
#define IS_SUB(node)                    node->value.l_type == SUB
#define IS_MUL(node)                    node->value.l_type == MUL
#define IS_DIV(node)                    node->value.l_type == DIV
#define IS_POW(node)                    node->value.l_type == POW
#define IS_SIN(node)                    node->value.l_type == SIN
#define IS_COS(node)                    node->value.l_type == COS
#define IS_CALL(node)                   node->value.l_type == CALL_FUNCTION
#define IS_ELSE(node)                   node->value.l_type == ELSE
#define IS_END_AC(node)                 node->value.l_type == END_AC
#define IS_VAR(node)                    node->value.l_type == VAR
#define IS_IF(node)                     node->value.l_type == IF
#define IS_WHILE(node)                  node->value.l_type == WHILE
#define IS_RET(node)                    node->value.l_type == RET
#define IS_SCAN(node)                   node->value.l_type == SCAN
#define IS_OUT(node)                    node->value.l_type == OUT

#define IS_BEGIN_COND(node)             node->value.l_type == BEGIN_COND
#define IS_END_COND(node)               node->value.l_type == END_COND
#define IS_CMP(node)                    (node->value.l_type == EQ_EQ || node->value.l_type == LESS_EQ || node->value.l_type == MORE_EQ || node->value.l_type == LESS ||\
                                        node->value.l_type == MORE || node->value.l_type == NOT_EQ)

#define IS_AND_OR(node)                 node->value.l_type == AND || node->value.l_type == OR

#define NEEDED_NODE(node)               node->value.l_type == VAR || node->value.l_type == IF || node->value.l_type == WHILE || node->value.l_type == CALL_FUNCTION ||\
                                        node->value.l_type == RET || node->value.l_type == SCAN || node->value.l_type == OUT || node->value.l_type == SKIP

#define SSaC() Skip_Spaces_Comments(text, &pointer, &current_line)

#define Syntax_Assert(condition) if(!(condition)) Syntax_Error(current_line)