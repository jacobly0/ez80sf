	assume	adl=1

	section	.text
	public	__lshrs_1_fast
__lshrs_1_fast: ; CHECK: bitcast(int32_t, pair8_24_t, { out.HL, out.E }) == bitcast(int32_t, pair8_24_t, { in.HL, in.E }) >> 1 && out.IX == in.IX
	push	hl
	ld	hl, 2
	add	hl, sp
	sra	e
	rr	(hl)
	pop	hl
	rr	h
	rr	l
	ret
