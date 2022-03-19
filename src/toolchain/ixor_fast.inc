	assume	adl=1

	section	.text
	public	__ixor_fast
__ixor_fast: ; CHECK: out.HL == (in.HL ^ in.BC) && out.IX == in.IX
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
