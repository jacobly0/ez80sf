	assume	adl=1

	section	.text
	public	__sshl
__sshl := __ishl ; PREREQ: in.C < 16 CHECK: out.HLs == (uint16_t)(in.HLs << in.C) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY

	extern	__ishl
