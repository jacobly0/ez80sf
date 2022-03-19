	assume	adl=1

	section	.text
	public	__inot_fast
__inot_fast: ; CHECK: out.HL == (~in.HL & 0xFFFFFF) && out.IX == in.IX
	add	hl, de	; uhl=UHL+UDE
	ex	de, hl	; ude=UHL+UDE, uhl=UDE
	scf
	sbc	hl, de	; uhl=UDE-(UHL+UDE)-1
			;    =UDE-UHL-UDE-1
			;    =-UHL-1
			;    =~UHL
	ret
