	assume	adl=1

	section	.text
	public	__land_fast
__land_fast: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == (bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) & bitcast(uint32_t, pair8_24_t, { in.BC, in.A })) && out.IX == in.IX
	and	a, e
	ld	e, a
	push	bc
	inc	sp
	inc	sp
	push	hl
	ld	hl, 2
	add	hl, sp
	ld	a, (hl)
	inc	hl
	and	a, (hl)
	dec	hl
	ld	(hl), a
	pop	hl
	inc	sp
	ld	a, h
	and	a, b
	ld	h, a
	ld	a, l
	and	a, c
	ld	l, a
	ret
