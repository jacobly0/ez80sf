	assume	adl=1

	section	.text
	public	__ior_fast
__ior_fast: ; CHECK: out.HL == (in.HL | in.BC) && out.IX == in.IX
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
	ret
