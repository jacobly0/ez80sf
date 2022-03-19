	assume	adl=1

	section	.text
	public	__llshrs_1_fast
__llshrs_1_fast: ; CHECK: bitcast(int64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) >> 1 && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	push	hl
	push	de
	ld	hl, 2
	add	hl, sp
	sra	b
	rr	c
	rr	(hl)
	pop	de
	inc	hl
	inc	hl
	inc	hl
	rr	d
	rr	e
	rr	(hl)
	pop	hl
	rr	h
	rr	l
	ret
