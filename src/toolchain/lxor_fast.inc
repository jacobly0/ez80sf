	assume	adl=1

	section	.text
	public	__lxor_fast
__lxor_fast: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == (bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) ^ bitcast(uint32_t, pair8_24_t, { in.BC, in.A })) && out.IX == in.IX
	xor	a, e
	ld	e, a
	push	bc
	inc	sp
	inc	sp
	push	hl
	ld	hl, 2
	add	hl, sp
	ld	a, (hl)
	inc	hl
	xor	a, (hl)
	dec	hl
	ld	(hl), a
	pop	hl
	inc	sp
	ld	a, h
	xor	a, b
	ld	h, a
	ld	a, l
	xor	a, c
	ld	l, a
	ret
