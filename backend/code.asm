call main
hlt


main:

push 1000
push 1000
div

pop [0]

push 7000
push 1000
div

pop [1]
in
pop [1]
push [1]

push 2000
push 1000
div

jb if_label_0
jmp if_label_1
if_label_0:

push 1000
push 1000
div

pop [0]
jmp if_label_2
if_label_1:

push 1000
push 1000
div

pop [2]
if_label_3:
push [2]
push [1]
ja if_label_4

push [0]
push [2]
mul
pop [0]
push [2]

push 1000
push 1000
div

add
pop [2]
jmp if_label_3
if_label_4:
if_label_2:
push [0]
out

push 0
push 1000
div

pop [999]
RET
