# Architecture Information

## Registers

There are 16 registers, numbered [0, 15]\
Technically, registers 1-13 are general purpose, but the convention is layed out as follows:

| ID     | Purpose                 |
| --     | -------                 |
| r0/zr  | Always 0                |
| r1     | General Purpose         |
| *...*  | General Purpose         |
| r9     | General Purpose         |
| r10/usp| Upper **Stack** Pointer |
| r11/lsp| Lower **Stack** Pointer |
| r12/ura| Upper **Return** Addr   |
| r13/lra| Lower **Return** Addr   |
| r14/uma| Upper **Memory** Addr   |
| r15/lma| Lower **Memory** Addr   |
