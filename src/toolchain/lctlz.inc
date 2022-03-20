	assume	adl=1

	section	.text
	public	__lctlz
__lctlz: ; CHECK: out.A == (bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) ? __builtin_clz(bitcast(uint32_t, pair8_24_t, { in.HL, in.E })) : 32) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	de
	push	hl

	scf
	sbc	a, a

.loop:
	adc	hl, hl
	rl	e
	inc	a
	jr	nc, .loop

	pop	hl
	pop	de
	ret
