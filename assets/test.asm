cls MEMCPY

imm r1, 2
imm r9, LOOP
jmp r0, r9

MEMCPY:
	qim r3, r4, 0x00010000
	qim r5, r6, 0x20000000
	
	MEMCPY_LOOP:
	mov uma, r3
	mov lma, r4
	g16 r7
	mov uma, r5
	mov lma, r6
	s16 r7

	scf ~
	adc r4, r4, zr
	adc r3, r3, zr
	scf ~
	adc r6, r6, zr

	imm r2, 8
	sub r2, r6, r2
	jnz MEMCPY_LOOP, r2
	
	ret

LOOP:
	qim uma, lma, 0x31000002
	set r1
	scf ~
	adc r1, r1, r0

	drw ~

	;imm r8, 30000
	;STALL:
	;	scf ~
	;	swc r8, r8, r0
	;	jnc STALL

	jmp r0, r9

;CLL #a, #b:
;	qim ura, lra, cur+10
;	jmp #a, #b

;CLS #a:
;	qim ura, lra, cur+14
;	scf ~
;	jcf #a

;RET:
;	jmp ura, lra

. 0x30000000 ; draw 1 sprite & stop
$ 0x8000

. 0x31000000 ; tile & pos
$ 0x0000 $ 0x0202

. 0x32000000 ; palette colors
$ 0x0000 $ 0x00FF $ 0xFF00

. 0x00010000 ;smiley face
$ 0x0001 $ 0x0101 $ 0x0101 $ 0x0100
$ 0x0101 $ 0x0101 $ 0x0101 $ 0x0101
$ 0x0101 $ 0x0001 $ 0x0100 $ 0x0101
$ 0x0101 $ 0x0001 $ 0x0100 $ 0x0101
$ 0x0101 $ 0x0101 $ 0x0101 $ 0x0101
$ 0x0100 $ 0x0101 $ 0x0101 $ 0x0001
$ 0x0101 $ 0x0000 $ 0x0000 $ 0x0101
$ 0x0001 $ 0x0101 $ 0x0101 $ 0x0100