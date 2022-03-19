	assume	adl=1

	section	.text
	public	__ineg_fast
__ineg_fast: ; PREREQ: in.HL != 1 << 23 CHECK: ((int32_t)(out.HL << 8) >> 8) == ((int32_t)-(in.HL << 8) >> 8) && out.IX == in.IX
	add	hl, de	; uhl=UHL+UDE
	ex	de, hl	; ude=UHL+UDE, uhl=UDE
	or	a, a
	sbc	hl, de	; uhl=UDE-(UHL+UDE)
			;    =UDE-UHL-UDE
			;    =-UHL
	ret
