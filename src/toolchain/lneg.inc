	assume	adl=1

	section	.text
	public	__lneg
__lneg: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == -bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) && out.A == in.A && out.BC == in.BC && out.D == in.D && out.IX == in.IX && out.IY == in.IY
	push	af
	push	de
	ex	de, hl	; ude=UHL, l=E
	xor	a, a	; a=0
	sbc	hl, hl	; uhl=0
	sbc	hl, de	; uhl=-UHL
	pop	de	; e=E
	sbc	a, e
	ld	e, a	; euhl=-EUHL
	pop	af
	ret
