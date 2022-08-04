	assume	adl=1

	section	.text
	public	__icmpzero
__icmpzero: ; CHECK: out.flags.Z == !in.HL && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	add	hl, de
	or	a, a
	sbc	hl, de
	ret
