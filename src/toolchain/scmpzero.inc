	assume	adl=1

	section	.text
	public	__scmpzero
__scmpzero: ; CHECK: out.flags.Z == !in.HLs && out.A == in.A && out.BC == in.BC && out.DES == in.DES && out.HLs == in.HLs && out.IX == in.IX && out.IY == in.IY
	add	hl, de
	or	a, a
	sbc.s	hl, de
	ret
