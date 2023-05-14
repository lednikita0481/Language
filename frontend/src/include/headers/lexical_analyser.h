#include "Language.h"
#include "Tree.h"
#include "DSL.h"

size_t Calculate_Hash(void* Structure, size_t size_bites);
void Lexic_Error(int line);
Node* Lexical_Tree_Connector(Node** nodes, int Tokens_Amount);
void Get_Func_Name(Node** nodes, int* cur_token, Node* func_node);
void Get_Parametrs(Node** nodes, int* cur_token, Node* func_node, Node* name_node);
Node* Get_Param(Node** nodes, int* cur_token, Node* func_node);
Node* Get_Statement_Exp(Node** nodes, int* cur_token);
Node* Get_Statement(Node** nodes, int* cur_token);
Node* Get_Action(Node** nodes, int* cur_token);
Node* GetE(Node** nodes, int* cur_token);
Node* GetU(Node** nodes, int* cur_token);
Node* GetT(Node** nodes, int* cur_token);
Node* GetW(Node** nodes, int* cur_token);
Node* GetP(Node** nodes, int* cur_token);
Node* GetN(Node** nodes, int* cur_token);