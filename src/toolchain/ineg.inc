	assume	adl=1

	section	.text
	public	__ineg
__ineg: ; PREREQ: in.HL != 1 << 23 CHECK: ((int32_t)(out.HL << 8) >> 8) == ((int32_t)-(in.HL << 8) >> 8) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
if 0
	push	de
end if
	add	hl, de	; uhl=UHL+UDE
	ex	de, hl	; ude=UHL+UDE, uhl=UDE
	or	a, a
	sbc	hl, de	; uhl=UDE-(UHL+UDE)
			;    =UDE-UHL-UDE
			;    =-UHL
if 0
	pop	de
else
	ex	de, hl	; ude=-UHL, uhl=UHL+UDE
	add	hl, de	; uhl=UHL+UDE+-UHL
			;    =UDE
	ex	de, hl	; ude=UDE, uhl=-UHL
end if
	ret
