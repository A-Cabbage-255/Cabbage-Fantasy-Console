imm uma, 0x3000
imm lma, 0x0000
imm r2, 0x80
set r2                       ; says to draw a sprite
imm lma, 0x0002
set zr                       ; ends sprites

imm uma, 0x3100
imm lma, 0x0000
s16 zr                       ; gives first sprite tile 0,0

imm lma, 0x0002
imm r1, 0x0010
s16 r1                       ; gives first sprite position 0,16

imm uma, 0x2000
imm lma, 0x0000
imm r5, 1
set r5

imm uma, 0x3200
imm lma, 0x0003
s16 uma

LOOP:
drw ~

imm uma, 0x3100
imm lma, 0x0002
get r1

imm r2, 1
add r1, r1, r2

set r1

imm lsp, LOOP
jmp zr, lsp