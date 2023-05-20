# My language and binary translator in JIT implementation

## Background

This repository consists of several works I've done during the 1st grade programming cource of [Ilya Dedinsky](https://github.com/ded32)
in MIPT DREC. The first one, [Processor](https://github.com/lednikita0481/Language/tree/main/backend/Processor), was implemented to understand 
the main principles of computer's work with help of simulation of the translation from assembler to byte code and its execution.

Some tasks later Language was implemented. It's possible to write something in it and after that it will be translated to our hand-made assembler. 
Then assembler translates the code to byte-code for our processor and it can be executed. Language consists of two subtasks. Firstly in 
[frontend](https://github.com/lednikita0481/Language/tree/main/frontend) the program is parsing my language code into the nodes tree 
and writes it to the file according to the [tree standart](https://github.com/dodokek/LanguageStandart/blob/main/README.md). After that 
[middlend part](https://github.com/lednikita0481/Language/tree/main/middlend) translates it to the assembly. Such division made our languages
"multi-platform": you can give your tree-standart file to another student, and your code will be executable on his realization of processor and vice versa.

Final task is a [binary translator](https://github.com/lednikita0481/Language/tree/main/JIT). It helps to make the code, written in my programming language, executable on a real machine. 

After the end of MIPT session I will add more detailed README about the language and to improve it a bit, but now I've described only binary translation.



## Binary Translation (JIT version)

My [program](https://github.com/lednikita0481/Language/tree/main/JIT) takes the binary code, generated by our assembler implemetation, translates it to x86-64 machine code and executes. For this purpose JIT uses specially allocated buffer. With help of *mprotect* the buffer gets execution rights and after that can be called as an average C function. 

To build the programm, you can put the binary file to the *programs* folder and write еру file name as a command line argument, for example: 
~~~ 
~./JIT factorial.prog
~~~

Or, if you want to see coming all the way from my language code to execution on x86-64 machine, put language code in file "/frontend/lang_code/code.txt" and call 
~~~ 
~ make run
~~~
in the main directory. In that way program will be executed twice: firstly on my Processor, secondly on x86-64 machine after the binary translation.

### Translation

Here you can see how commands are translated from my assebler to x86-64:

<details>
  <summary>Translation of Push & Pop </summary>
  
  | My assembler  |  x86-64 assembler           |   Comments               |
  | :-----------: | :-------------------------: | :----------------------: |
  | push [mem]    | push [r15 + mem]            | r15 - buffer begin       |
  | push num      | mov rax, num <br> push rax  | no command to push constant |
  | pop [mem]     | pop  [r15 + mem]            |r15 - buffer begin       |
  
  During language to asm translation process no other push & pop variations were used (that's not perfect and I will change it soon)
</details>

<details>
  <summary>Translation of arithmetics </summary>
  
  | My assembler  |  x86-64 assembler                                                      |   Comments               |
  | :-----------: | :--------------------------------------------------------------------: | :----------------------: |
  | add           | pop rbx <br> pop rax <br> add rax, rbx <br> push rax                   | take arguments from the stack and add  |
  | sub           | pop rbx <br> pop rax <br> sub rax, rbx <br> push rax                   | take arguments from the stack and sub  |
  | mul           | pop rbx <br> pop rax <br> imul rax, rbx <br> CQO <br> mov rdi, ACCURACY <br> idiv rdi <br> push rax | ACCURACY - constant that helps to imitate <br> floating point numbers on integer system.|
  | div           | pop rbx <br> pop rax <br> mov rdi, ACCURACY <br> imul rax, rdi <br> CQO <br> idiv rbx <br> push rax | multiply by ACCURACY in advance to save accuracy|
  
</details>

<details>
  <summary>Translation of sqrt </summary>
  
  ~~~
  pop rax
  cvtsi2sd xmm0, rax        // convert integer in rax to floating in xmm0
  mov rax, ACCURACY
  cvtsi2sd xmm1, rax
  divpd xmm0, xmm1          // no ACCURACY needed while sqrt calculating
  sqrtd xmm0, xmm0          
  mulpd xmm0, xmm1          // return accuracy
  cvtsd2si rax, xmm0        // convert floating in xmm0 to integer in rax
  push rax
  ~~~
  
</details>

<details>
  <summary>Translation of in </summary>
  
  ~~~
  lea rdi, [r15 + var]        // var - definitely not using memory cell in the buffer, r15 - buffer beginning
  pusha
  mov rbp, rsp
  and rsp, -16                // align the stack
  
  call In_Func                // C function, a wrapper of scanf
  
  mov rsp, rbp
  popa
  push [r15 + var]
  ~~~
  
</details>

<details>
  <summary>Translation of out </summary>
  
  ~~~
  pop rdi
  pusha
  mov rbp, rsp
  and rsp, -16                // align the stack
  
  call Out_Func               // C function, a wrapper of printf
  
  mov rsp, rbp
  popa
  ~~~
  
</details>


<details>
  <summary>Translation of jumps and call </summary>
  
  | My assembler  |  x86-64 assembler           |
  | :-----------: | :-------------------------: |
  | jmp           | jmp                         | 
  | call          | call                        | 
  | cond_jmp      | pop rbx <br> pop rax <br> cmp rax, rbx <br> cond_jmp  |
  
  In every argument we replace the absolute ip in the old byte code with a relative offset to the call address.
</details>

Firstly I translate given byte-code comands to structures, making so-called IR (Intermediate representation). It helps to translate byte code much easier, makes debugging more convenient and code more readable.


## Performance analyzing

Now we can compare the performance on hand-made processor and on real x86-64 architecture. In the time-test regime all outputs are shutted down and inputs are replaced for receiving a fixed number. Program is executed 10000000 times. You can change these parametres in "common_define.h"

**Factorial**:
- program counts 7!
- My Processor execution time: 7.48 s
- x86_64 execution time: 0.92 s
- Speed up: 8.1 times

**Quadratic equation solver**:
- programm counts $x^2 + 4x + 3 = 0$
- My Processor execution time: 6.38 s
- x86_64 execution time: 0.91 s
- Speed up: 7.0 times

## Conclusion 

During the implementation of these works I learned a lot about computer architecture, compilators and programming. In the nearest future, after the session end, I'm planning to improve my program, add some optimisations and implement more functions. I give all the thanks to Ded, aka Dedus, aka Ilya Dedinskiy, and to all my groupmates for this really interesting and productive year! Hope to continue our journey together!
