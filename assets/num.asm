qim USP, LSP, 0x00FFFFFF

qim UMA, LMA, 0xE00000C0
qim r1, r2, INTROUTINE
s16 r1
lim LMA, 0x00C2
s16 r2

imm r9, 0

imm r8, 1
lim r1, LOOP
LOOP:
	psl r1
	psl r8
	cls DRAWNUM
	ppl r8
	ppl r1
	drw ~
	jmp zr, r1

INTROUTINE:
	qim UMA, LMA, 0xF0000004
	get r4
	jez INTROUTINE_NOTYPE, r9
	imm r2, 3
	shl r2, r8, r2
	imm r3, 1
	shl r3, r8, r3
	add r8, r2, r3
	imm r3, 0x30
	;sub r4, r4, r3
	mov r8, r4
	imm r9, 0
	ccf ~
	jnc INTROUTINE_END
	INTROUTINE_NOTYPE:
	imm r3, 1
	sub r4, r4, r3
	jnz INTROUTINE_END, r4
	imm r9, 1
	INTROUTINE_END:
	rti ~

;input r8
;modifies r1-r3
;output r1 (result), r2 (remainder)
DIVIDE10:
	ccf ~
	imm r1, 1
	imm r2, 2
	shr r1, r8, r1
	shr r2, r8, r2
	add r1, r1, r2
	imm r2, 4
	shr r2, r1, r2
	add r1, r1, r2
	imm r2, 8
	shr r2, r1, r2
	add r1, r1, r2
	imm r2, 3
	shr r1, r1, r2

	imm r2, 2
	shl r2, r1, r2
	add r2, r2, r1
	imm r3, 1
	shl r2, r2, r3
	sub r2, r8, r2
	imm r3, 9
	sub r2, r2, r3
	ccf ~
	jle DIVIDE10_FINISH, r2
	
	imm r3, 1
	add r1, r1, r3
	
	DIVIDE10_FINISH:

	ccf ~

	imm r3, 2
	shl r2, r1, r3
	add r2, r1, r2
	imm r3, 1
	shl r2, r2, r3 ;r2 = r1 * 10

	sub r2, r8, r2

	ccf ~
	ret ~

; input r8
; modifies r1-r5, r8
; no output
DRAWNUM:
	imm r4, 5

	DRAWNUM_DIGITGEN:
	psl URA
	psl LRA
	cls DIVIDE10
	ppl LRA
	ppl URA

	imm r3, 0x30
	add r2, r2, r3
	psl r2

	mov r8, r1

	imm r2, 1
	sub r4, r4, r2
	jnz DRAWNUM_DIGITGEN, r4

	imm r4, 5
	qim UMA, LMA, 0xE3000000

	imm r5, 0

	DRAWNUM_DRAW:
	ppl r1

	imm r2, 0x30
	sub r2, r1, r2

	jnz DRAWNUM_NOSKIP, r2

	imm r2, 4
	sub r2, r5, r2

	jez DRAWNUM_NOSKIP, r2
	imm r2, 1
	sub r4, r4, r2
	scf ~
	adc r5, zr, r5
	ccf ~
	jnc DRAWNUM_DRAW
	DRAWNUM_NOSKIP:

	imm r5, 4
	set r1
	
	imm r2, 4
	add LMA, LMA, r2

	imm r2, 1
	sub r4, r4, r2
	jnz DRAWNUM_DRAW, r4


	ret ~

. 0xE2000000
$m,0x8080 
$m,0x8080 
$m,0x8080 
$m,0x8080 
$m,0x8080 
$m,0x8080
$m,0x0000

. 0xE3000000 ; tile & pos
$m,0x2000 $m,0x0000
$m,0x2000 $m,0x0800
$m,0x2000 $m,0x1000
$m,0x2000 $m,0x1800
$m,0x2000 $m,0x2000
$m,0x2000 $m,0x2800
$m,0x2000 $m,0x3000
$m,0x2000 $m,0x3800
$m,0x2000 $m,0x4000
$m,0x2000 $m,0x4800
$m,0x2000 $m,0x5000
$m,0x2000 $m,0x5800


$$"assets/font.asm"
