	assume	adl=1

	section	.text
	public	__lcmpu

if PREFER_OS_CRT

__lcmpu := 0001ACh

else

__lcmpu: ; CHECK: out.flags.Z == (bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) == bitcast(uint32_t, pair8_24_t, { in.BC, in.A })) && out.flags.C == (bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) < bitcast(uint32_t, pair8_24_t, { in.BC, in.A })) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	cp	a, e
	ccf
	ret	nz
	or	a, a
	sbc	hl, bc
	add	hl, bc
	ret

end if
