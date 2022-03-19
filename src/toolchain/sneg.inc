	assume	adl=1

	section	.text
	public	__sneg
__sneg := __ineg ; CHECK: out.HLs == (uint16_t)-in.HLs && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY

	extern	__ineg
