	assume	adl=1

	section	.text
	public	__lcmps_fast
__lcmps_fast: ; CHECK: (out.flags.PV ^ out.flags.S) == (bitcast(int32_t, pair8_24_t, { in.HL, in.E }) < bitcast(int32_t, pair8_24_t, { in.BC, in.A })) && out.IX == in.IX
	ld	d, a
	ld	a, e
	sub 	a, d
	ret	nz
	sbc	hl, bc
	sbc	a, a
	ret
