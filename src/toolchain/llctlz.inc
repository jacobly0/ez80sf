	assume	adl=1

	section	.text
	public	__llctlz
__llctlz: ; CHECK: out.A == (bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) ? __builtin_clzl(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS })) : 64) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	bc
	push	de
	push	hl

	scf
	sbc	a, a

.loop:
	adc	hl, hl
	ex	de, hl
	adc	hl, hl
	ex	de, hl
	rl	c
	rl	b
	inc	a
	jr	nc, .loop

	pop	hl
	pop	de
	pop	bc
	ret
