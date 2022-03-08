	assume	adl=1

	section	.text
	public	__bmuls
__bmuls: ; CHECK: out.A == (uint8_t)(in.A * in.B) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	bc
	ld	c,a
	mlt	bc
	ld	a,c
	pop	bc
	ret
