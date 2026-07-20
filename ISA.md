# Instruction Set

`r#` means "Register [#]"  
`CF` means Carry Flag

## ALU Mode

    0 QQQ DDDD AAAA BBBB

| QQQ | Name | Operation |
| - | - | - |
| `0` | add | `rD` = `rA` + `rB` |
| `1` | add w carry | `rD` = `rA` + `rB` + `CF` |
| `2` | sub w carry | `rD` = `rA` - `rB` - `CF` |
| `3` | sub | `rD` = `rA` - `rB` |
| `4` | bitwise nand | `rD` = ~(`rA` & `rB`) |
| `5` | multiply | `rH` : `rL` = `rA` * `rB` |
| `6` | shift left | `rD` = `rA` << `rB` |
| `7` | shift right | `rD` = `rA` >> `rB` |

**MULTIPLY** is a special operation, as the result is a 32-bit number. The instruction is interpreted as such:  
`0 QQQ HH LL AAAA BBBB`, where desination is split and the highest/lowest 16 bits are stored in `0HH`/`1LL`

> **NOTE:** if the destination is 0, the operation will be discarded, rendering the instruction as a `NOOP`

## Long Jump Mode

    101 U ---- HHHH LLLL

**`U`:** If enabled, the jump will bring the CPU into user-mode, and after 200 clock ticks an `INT 0x01` will run

immediately jumps to `rHHHH` : `rLLLL`

> **NOTE:** When `U` is enabled, this instruction can only be excuted in kernel mode

## Offset Jump Mode

    100 U QQQQ CCCC ---- VVVVVVVVVVVVVVVV

**`U`:** If enabled, the jump will bring the CPU into user-mode, and after 200 clock ticks an `INT 0x01` will run

jumps to current position + offset `V`, ***if*** the condition is met:

| QQQQ | Condition |
| - | - |
| `0` | `rC` = 0 |
| `1` | `rC` > 0 |
| `2` | `rC` < 0 |
| `3` | `rC` = `r1` |
| `4` | `CF` is set (= 1) |
| *5-7* | *unused* |
| `8` | `rC` != 0 |
| `9` | `rC` <= 0 |
| `10` | `rC` >= 0 |
| `11` | `rC` != `r1` |
| `12` | `CF` is off (= 0) |
| *13-15* | *unused* |

> **NOTE:** both `rCCCC` and `V` are interpreted as signed with two's complement
>
> **NOTE:** When `U` is enabled, this instruction can only be excuted in kernel mode

## RAM Mode

    110 L 0 P ------ RRRR

**`L`:** when off, operate with all 16-bits; when on, operate with the lower 8-bits  
with 16-bits, the upper 8 bits are in `r14` : `r15` and the lower 8 bits are in the adress right after

**`P`:**

- `0`: Get
- `1`: Store

Either gets or stores a value from `rRRRR`, with `r14` : `r15` as the adress.

## Software Interrupt

    110 - 1 --- IIIIIIII

Jumps to the IVT value at index `IIIIIIII`
> **Unless:**  
> if `I` is 0x00, computer shuts down  
> if `I` is 0xFE, rather than jumping the next frame is drawn and afterwards execution continues

## Short Immediate

    1110 DDDD NNNNNNNN

Stores value `NNNNNNNN` into `rDDDD`

## Long Immediate

    1111 DDDD -------- VVVVVVVVVVVVVVVV

Stores value `VVVVVVVVVVVVVVVV` into `rDDDD`
