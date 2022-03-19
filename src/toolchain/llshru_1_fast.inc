	assume	adl=1

	section	.text
	public	__llshru_1_fast
__llshru_1_fast: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) >> 1 && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	push	hl
	push	de
	ld	hl, 2
	add	hl, sp
	srl	b
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
