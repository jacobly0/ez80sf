	assume	adl=1

	section	.text
	public	__lnot_fast
__lnot_fast: ; CHECK: bitcast(int32_t, pair8_24_t, { out.HL, out.E }) == ~bitcast(int32_t, pair8_24_t, { in.HL, in.E }) && out.IX == in.IX
	ld	a, e	; a=E
	add	hl, de	; uhl=UHL+x
	ex	de, hl	; ude=UHL+x, uhl=x
	scf
	sbc	hl, de	; uhl=x-(UHL+x)-1
			;    =x-UHL-x-1
			;    =-UHL-1
			;    =~UHL
	cpl		; a=~E
	ld	e, a	; e=~E
	ret
