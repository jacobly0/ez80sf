	assume	adl=1

	section	.text
	public	__ibitrev_fast
__ibitrev_fast: ; CHECK: out.HL == __builtin_bitreverse32(in.HL) >> 8 && out.IX == in.IX
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	ld	c, a
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	ld	b, a
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	add	hl, hl
	rra
	push	af
	dec	sp
	pop	hl
	inc	sp
	ld	l, c
	ld	h, b
	ret
