# Assembly

*the following specification uses these shorthands **when describing instructions**:*  
*#n = register n*  
*[n] = memory at address n*  
*`n` = immediate number n*

> NOTE: An instruction with no arguments should have a `~` at the end

> NOTE: A negative number is always 16-bit 2's complement

## Non-instructions

### Label

`<identifier>:` - Specifies a label, where the following instruction's address is stored as a compile-time variable. IMM instructions will get the direct address while relative jumps will use the offset.

### Comment

`;<comment>` - A semicolon specifies a comment which continues until the end of the line. The compiler ignores everything in the comment. There are no multiline/inline comments.

## User Mode Instructions


| Code | Operation |
| - | - |
| `NOP ~` | Does nothing, equivalent to `ADD 0, 0, 0` |
| `MOV #d, #a` | Copies the value from register #a to register #d, equivalent to `ADD #d, #a, 0` |
| `ADD #d, #a, #b` | Adds the values in #a and #b and stores the result in #d |
| `ADC #d, #a, #b` | Adds the values in #a and #b, +1 if the carry bit is set, and stores the result #d |
| `SUB #d, #a, #b` | Subtracts the value in #b from #a and stores the result #d |
| `SWC #d, #a, #b` | Subtracts the value in #b from #a, -1 if the carry bit is set, and stores the result #d |
| `NND #d, #a, #b` | Bitwise NAND's the values in #a and #b and stores the result in #d |
| `MUL #h, #l, #a, #b` | Multiplies #a and #b, stores the higher 16 bits in #h (only if the result requires >16 bits) and lower 16 bits in #l.<br />#h must be from 0-3, #l must be from 4-7 |
| `SHL #d, #a, #b` | Shifts #a left by #b and stores the result #d |
| `SHR #d, #a, #b` | Shifts #a right by #b and stores the result #d |
| `JEZ #o, #c` | Jumps to immediate signed offset `o` if #c is equal to 0 |
| `JGZ #o, #c` | Jumps to immediate signed offset `o` if signed #c is greater than 0 |
| `JLZ #o, #c` | Jumps to immediate signed offset `o` if signed #c is less than 0 |
| `JE1 #o, #c` | Jumps to immediate signed offset `o` if #c is equal to #1 |
| `JNZ #o, #c` | Jumps to immediate signed offset `o` if #c is **not** equal to 0 |
| `JLE #o, #c` | Jumps to immediate signed offset `o` if signed #c is less than or equals 0 |
| `JGE #o, #c` | Jumps to immediate signed offset `o` if signed #c is greater than or equals 0 |
| `JN1 #o, #c` | Jumps to immediate signed offset `o` if #c is **not** equal to #1 |
| `JCF #o` | Jumps to immediate signed offset `o` if the carry flag is on |
| `JNC #o` | Jumps to immediate signed offset `o` if the carry flag is off |
| `JMP #h, #l` | Unconditionally jumps to #h:#l |
| `SET #v` | Writes the lower 8 bits of #v to [#14:#15] |
| `GET #v` | Gets the memory value at [#14:#15] and sends it to the lower 8 bits of #v |
| `S16 #v` | Writes the upper half of #v to [#14:#15], and lower half of #v to [#14:#15+1] (big-endian) |
| `G16 #v` | Gets the memory value at [#14:#15] & [#14:#15+1] and stores it in #v (big-endian) |
| `IMM #r, #v` | Sets register #r to value `v` |
| `LIM #r, #v` | Sets register #r to value `v`, guaruntees the long 32-bit version of the instruction |
| `INT #n` | For most values, a software interrupt with ID #n & changes processor to kernel mode |
| `DRW ~` | Causes the processor to stop as the screen is rendered, equivalent to `INT 0xFE` |
| `END ~` | Causes the console to shut down, equivalent to `INT 0x00` |
