#ifndef INCLUDED_L_H
#define INCLUDED_L_H

#include <stdio.h>
#include <stdlib.h>

struct Text
{
    char* buff;
    int symbols;
    int line;
};

enum Types
{
    VALUE_TYPE,
    VAR_TYPE,
    SPECIAL_TYPE,
    NAME_TYPE
};

enum Language_Types
{
    NOT_LANG_TYPE,
    SKIP,

    //арифметика
    ADD,//
    SUB,//
    DIV,//
    MUL,//
    POW,//
    SIN,//
    COS,//
    SQRT,

    //сравнение
    EQ_EQ,//
    LESS_EQ,//
    MORE_EQ,//
    LESS, //
    MORE,//
    NOT_EQ,//

    EQ,

    //логика
    VAR,
    IF,
    ELSE, 
    WHILE,
    START_EXP,//
    END_EXP,//
    END_AC,
    BEGIN_COND,//
    END_COND,//
    FUNC_D,
    FUNC_V,
    VOID,
    TYPE,
    CALL_FUNCTION,
    RET,
    SCAN, 
    OUT,
    AND,
    OR,

    //help types
    NIL,
    ST,
    PARAM
};

union lang_value
{
    double dval;
    Language_Types l_type;
    char*  name;
};


struct Node
{
    Types type;
    lang_value value;
    char* call_func_name;
    Node* left;
    Node* right;
    Node* parent;
    int code_line;
    int num;
};

struct Function
{
    Node* node;
    char* name;
    size_t hash;
};

#endif