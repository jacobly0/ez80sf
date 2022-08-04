	assume	adl=1

	section	.text
	public	__lcmpu_fast
__lcmpu_fast: ; CHECK: out.flags.Z == (bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) == bitcast(uint32_t, pair8_24_t, { in.BC, in.A })) && out.flags.C == (bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) < bitcast(uint32_t, pair8_24_t, { in.BC, in.A })) && out.IX == in.IX
	sub	a, e
	ccf
	ret	nz
	or	a, a
	sbc	hl, bc
	ret
