imm 14, 0x3000
imm 15, 0x0000
imm 2, 0x80
set 2                       ; says to draw a sprite
imm 15, 0x0002
set 0                       ; ends sprites

imm 14, 0x3100
imm 15, 0x0000
s16 0                       ; gives first sprite tile 0,0

imm 15, 0x0002
imm 1, 0x0010
s16 1                       ; gives first sprite position 0,16

imm 14, 0x2000
imm 15, 0x0000
imm 5, 1
set 5

imm 14, 0x3200
imm 15, 0x0003
s16 14

LOOP:
drw ~

imm 14, 0x3100
imm 15, 0x0002
get 1

imm 2, 1
add 1, 1, 2

set 1

add 0, 0, 0
jnc LOOP