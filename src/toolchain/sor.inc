	assume	adl=1

	section	.text
	public	__sor
__sor: ; CHECK: out.HLs == (in.HLs | in.BCS) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
	push	af
	ld	a, h
	or	a, b
	ld	h, a
	ld	a, l
	or	a, c
	ld	l, a
	pop	af
	ret
