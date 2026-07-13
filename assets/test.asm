imm 1, LOOP
imm 14, 0x3000
imm 15, 0x0000
imm 2, 0x80
set 2
imm 15, 0x0001
set 0
imm 14, 0x3100
imm 15, 0x0000
s16 0
imm 15, 0x0002
s16 0
LOOP:
drw ~
jmp 0, 1