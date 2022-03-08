	assume	adl=1

	section	.text
	public	__lpopcnt
__lpopcnt: ; CHECK: out.A == __builtin_popcount(bitcast(uint32_t, pair8_24_t, { in.HL, in.E })) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	bc
	ld	b, 4
	public	__lpopcnt.hijack1
.hijack1:
	push	hl
	xor	a, a
	ld	c, a
.loop:
	add	hl, hl
	adc	a, c
	add	hl, hl
	adc	a, c
	add	hl, hl
	adc	a, c
	add	hl, hl
	adc	a, c
	add	hl, hl
	adc	a, c
	add	hl, hl
	adc	a, c
	add	hl, hl
	adc	a, c
	add	hl, hl
	adc	a, c
	ld	l, e
	djnz	.loop
	pop	hl
	pop	bc
	ret
