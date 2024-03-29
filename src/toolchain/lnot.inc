	assume	adl=1

	section	.text
	public	__lnot
__lnot: ; CHECK: bitcast(int32_t, pair8_24_t, { out.HL, out.E }) == ~bitcast(int32_t, pair8_24_t, { in.HL, in.E }) && out.A == in.A && out.BC == in.BC && out.D == in.D && out.IX == in.IX && out.IY == in.IY
	push	af
	ld	a, e	; a=E
	cpl		; a=~E
	ld	e, a	; e=~E
	pop	af
	push	de
	add	hl, de	; uhl=UHL+x
	ex	de, hl	; ude=UHL+x, uhl=x
	scf
	sbc	hl, de	; uhl=x-(UHL+x)-1
			;    =x-UHL-x-1
			;    =-UHL-1
			;    =~UHL
	pop	de
	ret
