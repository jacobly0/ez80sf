	assume	adl=1

	section	.text
	public	__iand
__iand: ; CHECK: out.HL == (in.HL & in.BC) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
	push	af
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
	pop	af
	ret
