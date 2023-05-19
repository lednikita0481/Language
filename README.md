# My language and binary translator in JIT implementation

## Background

This repository consists of several works I've done during the 1st grade programming cource of [Ilya Dedinsky](https://github.com/ded32)
in MIPT DREC. The first one, [Processor](https://github.com/lednikita0481/Language/tree/main/backend/Processor) was implemented to understand 
the main principles of computer's work with help of simulation of the translation from assembler to byte code and its execution.

Some tasks later Language was implemented. It's possible to write something in it and after that it will be translated to our hand-made assembler. 
Then assembler translates the code to byte-code for our processor and it can be executed. Language consists of two subtasks. Firstly in 
[frontend](https://github.com/lednikita0481/Language/tree/main/frontend) the program is parsing my language code into the nodes tree 
and writes it to the file according to the [tree standart](https://github.com/dodokek/LanguageStandart/blob/main/README.md). After that 
[middlend part](https://github.com/lednikita0481/Language/tree/main/middlend) translates it to the assembly. Such division made our languages
"multi-platform": you can give your tree-standart file to another student, and your code will be executable on his realization of processor and vice versa.

Final task is a binary translator. It helps to make the code, written in my programming language, make executable on a real machine. 

After the end of MIPT session I will add more detailed README about the language, but now I've described only binary translation.

