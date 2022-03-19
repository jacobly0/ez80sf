	assume	adl=1

	section	.text
	public	__lsub_fast
__lsub_fast: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) - bitcast(uint32_t, pair8_24_t, { in.BC, in.A }) && out.IX == in.IX
	or	a, a
	sbc	hl, bc
	cpl
	ccf
	adc	a, e
	ld	e, a
	ret
