	assume	adl=1

	section	.text
	public	__llpopcnt
__llpopcnt: ; CHECK: out.A == __builtin_popcountl(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS })) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	call	__lpopcnt
	push	de
	push	hl
	ex	de, hl
	ld	l, b
	ld	e, c
	ld	d, a
	call	__lpopcnt
	add	a, d
	pop	hl
	pop	de
	ret

	extern	__lpopcnt
