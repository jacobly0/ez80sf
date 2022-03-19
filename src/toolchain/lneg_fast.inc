	assume	adl=1

	section	.text
	public	__lneg_fast
__lneg_fast: ; PREREQ: bitcast(int32_t, pair8_24_t, { in.HL, in.E }) != (int32_t)(1U << 31) CHECK: bitcast(int32_t, pair8_24_t, { out.HL, out.E }) == -bitcast(int32_t, pair8_24_t, { in.HL, in.E }) && out.IX == in.IX
	ld	c, e	; c=E
	ex	de, hl	; ude=UHL, l=E
	xor	a, a	; a=0
	sbc	hl, hl	; uhl=0
	sbc	hl, de	; uhl=-UHL
	sbc	a, c
	ld	e, a	; euhl=-EUHL
	ret
