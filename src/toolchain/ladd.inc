	assume	adl=1

	section	.text
	public	__ladd
__ladd: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) + bitcast(uint32_t, pair8_24_t, { in.BC, in.A }) && out.A == in.A && out.BC == in.BC && out.D == in.D && out.IX == in.IX && out.IY == in.IY
	push	af
	add	hl, bc
	adc	a, e
	ld	e, a
	pop	af
	ret
