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
                            {"положи в свой никчемный мозг:", 29, VAR}, //int
                            {"я передумал:", 12, SKIP},
                            {"это", 3, EQ}, // = 
                            {"ломай меня и делай что скажут пока", 34, WHILE}, // while
                            {"к сожалению, начнем", 19, START_EXP}, //{
                            {"ура, закончили", 14, END_EXP}, //}
                            {"(", 1, BEGIN_COND},
                            {")", 1, END_COND},
                            {"ну все хватит", 13, END_AC}, //;
                            {"не думай, просто делай и потом отдай, когда тебе скажут:", 56, FUNC_D}, //function 
                            {"не думай, просто делай, когда тебе скажут:", 42, FUNC_V},
                            {"без лишних слов отдай", 21, RET},//return
                            {"запихнуть", 9, SCAN}, //input
                            {"выпихнуть", 9, OUT}, //output
                            {"сломай меня и сделай это если", 29, IF},//if
                            {"не сломал? тогда", 16, ELSE},//else
                            {"<=", 2, LESS_EQ},
                            {">=", 2, MORE_EQ},
                            {"точно", 5, EQ_EQ},
                            {"!=", 2, NOT_EQ},
                            {">", 1, MORE},
                            {"<", 1, LESS},
                            {"добавъ", 6, ADD}, //+
                            {"убавъ", 5, SUB}, //-
                            {"умножъ", 6, MUL}, //*
                            {"поделиъ", 7, DIV}, // /
                            {"возведиъ", 8, POW}, // pow
                            {"просинусируйъ", 13, SIN},//sin
                            {"прокосинусируйъ", 15, COS}, //cos 
                            {"прокорнируйъ", 12, SQRT}, //cos 
                            {"еще", 3, AND},//&&
                            {"или", 3, OR}//||
                        };

#endif