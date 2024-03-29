	assume	adl=1

	section	.text
	public	__ipopcnt_fast
__ipopcnt_fast: ; CHECK: out.A == __builtin_popcount(in.HL) && out.IX == in.IX
	; Inlined implementation of __ipopcnt
	; Destroys: HL
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
	sbc	a, l
	add	hl, hl
	sbc	a, l
	add	hl, hl
	sbc	a, l
	add	hl, hl
	sbc	a, l
	add	hl, hl
	sbc	a, l
	add	hl, hl
	sbc	a, l
	add	hl, hl
	sbc	a, l
	add	hl, hl
	sbc	a, l

	add	hl, hl
	cpl
	adc	a, 9
	ret
