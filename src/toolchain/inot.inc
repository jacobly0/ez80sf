	assume	adl=1

	section	.text
	public	__inot
__inot: ; CHECK: out.HL == (~in.HL & 0xFFFFFF) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
if 0
	push	de
end if
	add	hl, de	; uhl=UHL+UDE
	ex	de, hl	; ude=UHL+UDE, uhl=UDE
	scf
	sbc	hl, de	; uhl=UDE-(UHL+UDE)-1
			;    =UDE-UHL-UDE-1
			;    =-UHL-1
			;    =~UHL
if 0
	pop	de
else
	ex	de, hl	; ude=~UHL, uhl=UHL+UDE
	add	hl, de	; uhl=UHL+UDE+~UHL
			;    =UHL+UDE+(-UHL-1)
			;    =UHL+UDE-UHL-1
			;    =UDE-1
	inc	hl	; uhl=UDE
	ex	de, hl	; ude=UDE, uhl=~UHL
end if
	ret
