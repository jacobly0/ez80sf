	assume	adl=1

	section	.text
	public	__llmulu_b
__llmulu_b: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) * *(uint8_t*)inStack[1]) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	push	af
	push	iy
	ld	iy, 0
	add	iy, sp
	push	de
	push	hl
	ld	a, (iy + 9)
	ld	h, a
	mlt	hl
	ld	(iy - 6), l
	ld	d, a
	ld	e, (iy - 5)
	mlt	de
	ld	l, h
	ld	h, 0
	add	hl, de
	ld	(iy - 5), l
	ld	d, a
	ld	e, (iy - 4)
	mlt	de
	ld	l, h
	ld	h, 0
	add	hl, de
	ld	(iy - 4), l
	ld	d, a
	ld	e, (iy - 3)
	mlt	de
	ld	l, h
	ld	h, 0
	add	hl, de
	ld	(iy - 3), l
	ld	d, a
	ld	e, (iy - 2)
	mlt	de
	ld	l, h
	ld	h, 0
	add	hl, de
	ld	(iy - 2), l
	ld	d, a
	ld	e, (iy - 1)
	mlt	de
	ld	l, h
	ld	h, 0
	add	hl, de
	ld	(iy - 1), l
	ld	d, a
	ld	e, c
	mlt	de
	ld	l, h
	ld	h, 0
	add	hl, de
	ld	c, l
	ld	d, a
	ld	e, b
	mlt	de
	ld	a, h
	add	a, e
	ld	b, a
	pop	hl
	pop	de
	pop	iy
	pop	af
	ret
