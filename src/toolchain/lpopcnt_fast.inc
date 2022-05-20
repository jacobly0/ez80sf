	assume	adl=1

	section	.text
	public	__lpopcnt_fast
__lpopcnt_fast: ; CHECK: out.A == __builtin_popcount(bitcast(uint32_t, pair8_24_t, { in.HL, in.E })) && out.IX == in.IX
	; Inlined implementation of __lpopcnt
	push	hl
	ld	a, l
	cpl
	ld	l, a
	sub	a, h
	sub	a, h

	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h

	add	hl, hl
	ld	h, a
	ld	a, e
	cpl
	ld	l, a
	sbc	a, h

	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h
	add	hl, hl
	sbc	a, h

	add	hl, hl
	cpl
	adc	a, 17
	pop	hl
	ret
