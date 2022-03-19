	assume	adl=1

	section	.text
	public	__ladd_b
__ladd_b: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) + in.A && out.A == in.A && out.BC == in.BC && out.D == in.D && out.IX == in.IX && out.IY == in.IY
	push	bc
	inc.s	bc
	ld	c, a
	ld	b, 0
	add	hl, bc
	pop	bc
	ret	nc
	inc	e
	ret
