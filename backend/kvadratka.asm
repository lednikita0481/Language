call main
hlt


main:

push 0
push 1000
div

pop [0]

push 0
push 1000
div

pop [1]

push 0
push 1000
div

pop [2]

push 0
push 1000
div

pop [3]

push 0
push 1000
div

pop [4]
in
pop [0]
in
pop [1]
in
pop [2]
push [0]

push 0
push 1000
div

je if_label_0
jmp if_label_1
if_label_0:
push [1]
push [2]
call хрень_без_палочки
push[999]
jmp if_label_2
if_label_1:
push [0]
push [1]
push [2]
call хрень_с_палочкой
push[999]
if_label_2:

push 0
push 1000
div

pop [999]
RET
хрень_без_палочки:
push [1]

push 0
push 1000
div

jne if_label_3
jmp if_label_4
if_label_3:

push 0
push 1000
div


push 1000
push 1000
div

sub
push [2]
mul
push [1]
div
pop [3]
jmp if_label_5
if_label_4:
push [2]

push 0
push 1000
div

je if_label_6
jmp if_label_7
if_label_6:

push 888000
push 1000
div

pop [3]
jmp if_label_8
if_label_7:

push 0
push 1000
div


push 911000
push 1000
div

sub
pop [3]
if_label_8:
if_label_5:
push [3]
out

push 0
push 1000
div

pop [999]
RET
хрень_с_палочкой:
push [1]
push [1]
mul

push 4000
push 1000
div

push [0]
mul
push [2]
mul
sub
pop [5]
push [5]
out
push [5]

push 0
push 1000
div

jb if_label_9
jmp if_label_10
if_label_9:

push 0
push 1000
div


push 911000
push 1000
div

sub
pop [6]
push [6]
out
jmp if_label_11
if_label_10:

push 0
push 1000
div

push [1]
sub
push [5]
sqrt
sub

push 2000
push 1000
div

push [0]
mul
div
pop [3]

push 0
push 1000
div

push [1]
sub
push [5]
sqrt
add

push 2000
push 1000
div

push [0]
mul
div
pop [4]
push [3]
out
push [4]
out
if_label_11:

push 0
push 1000
div

pop [999]
RET
