# Architecture Information

*[back to readme](README.md)*

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

## Interrupts

\* = not an interrupt; cpu doesn't jump when called but rather performs a special operation  
! = not an interrupt; if called, cpu will instead do int 0xFF

| ID(hex) | Purpose                                          |
| -       | -                                                |
| 0 *     | Halt cpu/shutoff                                 |
| 1       | normal interrupt                                 |
| *...*   | normal interrupt                                 |
| DF      | normal interrupt                                 |
| E0 !    | usr mem access low bound                         |
| E1 !    | usr mem access high bound                        |
| E2 !    | Sets the keyboard input address                  |
| E3      | reserved                                         |
| *...*   | reserved                                         |
| FD      | reserved                                         |
| FE *    | instructs computer to draw next frame            |
| FF      | called when kernel action is attemped in usr mode|
