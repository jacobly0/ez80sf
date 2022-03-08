	assume	adl=1

	section	.text
	public	__ipopcnt
__ipopcnt: ; CHECK: out.A == __builtin_popcount(in.HL) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	bc
	ld	b, 3
	jp	__lpopcnt.hijack1

	extern	__lpopcnt.hijack1
