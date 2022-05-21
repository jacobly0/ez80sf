	assume	adl=1

	section	.text
	public	__spopcnt_fast
__spopcnt_fast: ; CHECK: out.A == __builtin_popcount(in.HLs) && out.IX == in.IX
	; Inlined implementation of __spopcnt
	; Destroys: HL
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl

	ld	a, h
	ld	h, l
	cpl
	ld	l, a

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
	adc	a, 9
	ret
