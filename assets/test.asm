imm usp, 0x00FF
imm lsp, 0xFFFF

imm r2, 57
imm r3, 33
imm r4, 1
ccf ~
jnc PUSHES

.0x1000

POPS:
pop r7
pop r6
pop r5
ccf ~
jnc FIN

imm r6, 65535

PUSHES:
psh r2
psh r3
psh r4
ccf ~
jnc POPS

FIN:
end ~