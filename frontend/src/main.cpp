#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <locale.h>

#include "include/headers/Language.h"
#include "include/headers/File_work.h"
#include "include/headers/tokeniser.h"
#include "include/headers/dump.h"
#include "include/headers/lexical_analyser.h"

int main(const int argc, const char* argv[])
{
    setlocale(LC_ALL, "Rus");
    Text text = {};
    File_Read(&text, argc, argv);
    Node** nodes = (Node**) calloc(text.symbols, sizeof(Node*));
    int nodes_amount = Tokenise(nodes, &text);
    Node* Main_Node = Lexical_Tree_Connector(nodes, nodes_amount);
    Proga_Dump(Main_Node);
    Standart_Dump(argc, argv, Main_Node);
}