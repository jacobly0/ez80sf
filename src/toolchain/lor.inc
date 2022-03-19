	assume	adl=1

	section	.text
	public	__lor
__lor: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == (bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) | bitcast(uint32_t, pair8_24_t, { in.BC, in.A })) && out.A == in.A && out.BC == in.BC && out.D == in.D && out.IX == in.IX && out.IY == in.IY
	push	af
	or	a, e
	ld	e, a
	push	bc
	inc	sp
	inc	sp
	push	hl
	ld	hl, 2
	add	hl, sp
	ld	a, (hl)
	inc	hl
	or	a, (hl)
	dec	hl
	ld	(hl), a
	pop	hl
	inc	sp
	ld	a, h
	or	a, b
	ld	h, a
	ld	a, l
	or	a, c
	ld	l, a
	pop	af
	ret
