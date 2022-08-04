	assume	adl=1

	section	.text
	public	__lcmpzero
__lcmpzero: ; CHECK: out.flags.Z == !bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	inc	e
	dec	e
	ret	nz
	add	hl, bc
	or	a, a
	sbc	hl, bc
	ret
