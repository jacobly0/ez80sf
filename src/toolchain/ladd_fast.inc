	assume	adl=1

	section	.text
	public	__ladd_fast
__ladd_fast: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) + bitcast(uint32_t, pair8_24_t, { in.BC, in.A }) && out.IX == in.IX
	add	hl, bc
	adc	a, e
	ld	e, a
	ret
