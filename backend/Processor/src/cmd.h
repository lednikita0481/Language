DEF_CMD(hlt, 0, 0,
    {
        return;
    })

DEF_CMD(push, 1, 1,
    {
        int* arg_shit = Get_arg(cpu);
        int arg = 0;
        if (((*(cpu->code + cpu->ip - sizeof(char) - sizeof(int)) & (1 << imm_const)) != 0) && 
            ((*(cpu->code + cpu->ip - sizeof(char) - sizeof(int)) & (1 << memory)) == 0))
        {
            arg = ((*(int*)(&arg_shit)))*ACCURACY;
        }
        else
        {
            arg = *arg_shit;
        }
        StackPush(&cpu->stk, arg);
    })

DEF_CMD(pop, 2, 1,
    {
        int* arg = Get_arg(cpu);
        int element = StackPop(&cpu->stk);
        *(arg) = element;
    })

DEF_CMD(add, 3, 0,
    {
        StackPush(&cpu->stk, StackPop(&cpu->stk) + StackPop(&cpu->stk));
        cpu->ip += sizeof(char);
    })

DEF_CMD(sub, 4, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        StackPush(&cpu->stk, b - a);
        cpu->ip += sizeof(char);
    })

DEF_CMD(mul, 5, 0, 
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        StackPush(&cpu->stk, (((a/ACCURACY) * (b))));
        cpu->ip += sizeof(char);
    })

DEF_CMD(div, 6, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        if (a == 0)
        {
            printf("impossible to divide by zero");
            exit(DIV_BY_ZERO);
        }
        StackPush(&cpu->stk, (int)(((double)b / (double)a)*ACCURACY));
        cpu->ip += sizeof(char);
    })

DEF_CMD(jmp, 7, 0,
    {
        cpu->ip = *((int*)(cpu->code + cpu->ip + sizeof(char)));
    })

DEF_CMD(jb, 8, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        if (b < a) cpu->ip = *((int*)(cpu->code + cpu->ip + sizeof(char)));
        else cpu->ip += sizeof(char) + sizeof(int);
        //StackPush(&cpu->stk, b);
        //StackPush(&cpu->stk, a);
    })

DEF_CMD(jbe, 9, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        if (b <= a) cpu->ip = *((int*)(cpu->code + cpu->ip + sizeof(char)));
        else cpu->ip += sizeof(char) + sizeof(int);
        //StackPush(&cpu->stk, b);
        //StackPush(&cpu->stk, a);
    })

DEF_CMD(ja, 10, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        if (b > a) cpu->ip = *((int*)(cpu->code + cpu->ip + sizeof(char)));
        else cpu->ip += sizeof(char) + sizeof(int);
        //StackPush(&cpu->stk, b);
        //StackPush(&cpu->stk, a);
    })

DEF_CMD(jae, 11, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        if (b >= a) cpu->ip = *((int*)(cpu->code + cpu->ip + sizeof(char)));
        else cpu->ip += sizeof(char) + sizeof(int);
        //StackPush(&cpu->stk, b);
        //StackPush(&cpu->stk, a);
    })

DEF_CMD(je, 12, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        if (b == a) cpu->ip = *((int*)(cpu->code + cpu->ip + sizeof(char)));
        else cpu->ip += sizeof(char) + sizeof(int);
        //StackPush(&cpu->stk, b);
        //StackPush(&cpu->stk, a);
    })

DEF_CMD(jne, 13, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        if (b != a) cpu->ip = *((int*)(cpu->code + cpu->ip + sizeof(char)));
        else cpu->ip += sizeof(char) + sizeof(int);
        //StackPush(&cpu->stk, b);
        //StackPush(&cpu->stk, a);
    })

DEF_CMD(call, 14, 0,
    {
        StackPush(&cpu->call_adress_stk, (int)(cpu->ip + sizeof(char) + sizeof(int)));
        cpu->ip = *((int*)(cpu->code + cpu->ip + sizeof(char)));
    })

DEF_CMD(ret, 15, 0,
    {
        cpu->ip = (int)StackPop(&cpu->call_adress_stk);
    })

DEF_CMD(in, 16, 0,
    {
        int arg = 0;
        #ifndef TIME_TEST
        printf("Enter a number: ");
        scanf("%d", &arg);
        #endif

        #ifdef TIME_TEST
        arg = COMMON_IN;
        #endif
        StackPush(&cpu->stk, arg * ACCURACY);
        cpu->ip += sizeof(char);
    })

DEF_CMD(out, 17, 0,
    {
        double a = StackPop(&cpu->stk);
        #ifndef TIME_TEST
        printf("%lf\n", (a / ACCURACY));
        #endif
        cpu->ip += sizeof(char);
    })

DEF_CMD(sqrt, 18, 0, 
    {
        int a = StackPop(&cpu->stk);
        int b = (int)(ACCURACY * sqrt(a / ACCURACY));
        StackPush(&cpu->stk, b);
        cpu->ip += sizeof(char);
    })

DEF_CMD(ram, 19, 0, 
    {
        Print_Memory(cpu);
        cpu->ip += sizeof(char);
    })

DEF_CMD(mod, 20, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        int c = (b/ACCURACY) % (a/ACCURACY);
        if (a == 0)
        {
            printf("impossible to divide by zero");
            exit(DIV_BY_ZERO);
        }
        StackPush(&cpu->stk, c*ACCURACY);
        cpu->ip += sizeof(char);
    })

DEF_CMD(divv, 21, 0,
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        if (a == 0)
        {
            printf("impossible to divide by zero");
            exit(DIV_BY_ZERO);
        }
        StackPush(&cpu->stk, (b / a)*ACCURACY);
        cpu->ip += sizeof(char);
    })

DEF_CMD(copy, 22, 0, 
    {
        int a = StackPop(&cpu->stk);
        StackPush(&cpu->stk, a);
        StackPush(&cpu->stk, a);
        cpu->ip += sizeof(char);
    })

DEF_CMD(pow, 23, 0, 
    {
        int a = StackPop(&cpu->stk);
        int b = StackPop(&cpu->stk);
        int c = (int)(ACCURACY * pow(((double)a)/ACCURACY, ((double)b)/ACCURACY));
        StackPush(&cpu->stk, c);
        cpu->ip += sizeof(char);
    })

DEF_CMD(sin, 24, 0, 
    {
        int a = StackPop(&cpu->stk);
        int b = (int)(ACCURACY * sin(a / ACCURACY));
        StackPush(&cpu->stk, b);
        cpu->ip += sizeof(char);
    })

DEF_CMD(cos, 25, 0, 
    {
        int a = StackPop(&cpu->stk);
        int b = (int)(ACCURACY * cos(a / ACCURACY));
        StackPush(&cpu->stk, b);
        cpu->ip += sizeof(char);
    })