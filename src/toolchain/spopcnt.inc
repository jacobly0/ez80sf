	assume	adl=1

	section	.text
	public	__spopcnt
__spopcnt: ; CHECK: out.A == __builtin_popcount(in.HLs) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	hl
	ld	a,l
	call	__bpopcnt
	ld	l,a
	ld	a,h
	call	__bpopcnt
	add	a,l
	pop	hl
	ret

	extern	__bpopcnt
