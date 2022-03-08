	assume	adl=1

	section	.text
	public	__bmulu
__bmulu: ; CHECK: out.A == (uint8_t)(in.B * in.C) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	bc
	mlt	bc
	ld	a,c
	pop	bc
	ret
