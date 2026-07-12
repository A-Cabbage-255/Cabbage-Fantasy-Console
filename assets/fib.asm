;NOTE: DOES NOT FUNCTION. Currently, the console cannot output to the terminal.
;runs the fibonacci sequence, ends when it can no longer it in 16 bits

;imm 15, 1
;imm 1, 0
;imm 2, 1
;imm 7, LOOP
;imm 8, 8
;LOOP:
;	drw ~
;	add 15, 1, 2
;	jcf 8
;	add 1, 2, 0
;	add 2, 15, 0
;	jmp 0, 7
;FIN:
end ~