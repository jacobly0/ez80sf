	assume	adl=1

	section	.text
	public	__ladd_1
__ladd_1: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) + 1 && out.A == in.A && out.BC == in.BC && out.D == in.D && out.IX == in.IX && out.IY == in.IY
	inc	hl
	add	hl, bc
	or	a, a
	sbc	hl, bc
	ret	nz
	inc	e
	ret
