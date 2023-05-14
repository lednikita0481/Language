#include "Language.h"
#include "DSL.h"
#include "Tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

void Skip_Spaces_Comments(Text* text, int* pointer, int* current_line);
int Next_Word_Len(Text* text, int* pointer);
int isalpharus(char p);
char* Get_Lim_Len_String(char* to, const char* from, const int lendth);
double Get_Num(const char* str, int* pointer);

int Tokenise(Node** nodes, Text* text);