#ifndef INCLUDED_D_H
#define INCLUDED_D_H

struct code_string
{
    char* name;
    int lendth;
    Language_Types l_type;
};

const int COMMANDS_NUMBER = 31;

code_string COMMANDS[] = 
                        {
                            {"������ � ���� ��������� ����:", 29, VAR}, //int
                            {"� ���������:", 12, SKIP},
                            {"���", 3, EQ}, // = 
                            {"����� ���� � ����� ��� ������ ����", 34, WHILE}, // while
                            {"� ���������, ������", 19, START_EXP}, //{
                            {"���, ���������", 14, END_EXP}, //}
                            {"(", 1, BEGIN_COND},
                            {")", 1, END_COND},
                            {"�� ��� ������", 13, END_AC}, //;
                            {"�� �����, ������ ����� � ����� �����, ����� ���� ������:", 56, FUNC_D}, //function 
                            {"�� �����, ������ �����, ����� ���� ������:", 42, FUNC_V},
                            {"��� ������ ���� �����", 21, RET},//return
                            {"���������", 9, SCAN}, //input
                            {"���������", 9, OUT}, //output
                            {"������ ���� � ������ ��� ����", 29, IF},//if
                            {"�� ������? �����", 16, ELSE},//else
                            {"<=", 2, LESS_EQ},
                            {">=", 2, MORE_EQ},
                            {"�����", 5, EQ_EQ},
                            {"!=", 2, NOT_EQ},
                            {">", 1, MORE},
                            {"<", 1, LESS},
                            {"������", 6, ADD}, //+
                            {"�����", 5, SUB}, //-
                            {"������", 6, MUL}, //*
                            {"�������", 7, DIV}, // /
                            {"��������", 8, POW}, // pow
                            {"�������������", 13, SIN},//sin
                            {"���������������", 15, COS}, //cos 
                            {"������������", 12, SQRT}, //cos 
                            {"���", 3, AND},//&&
                            {"���", 3, OR}//||
                        };

#endif