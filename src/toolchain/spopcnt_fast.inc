	assume	adl=1

	section	.text
	public	__spopcnt_fast
__spopcnt_fast: ; CHECK: out.A == __builtin_popcount(in.HLs) && out.IX == in.IX
	push	hl
	xor	a, a
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	add	hl, hl
	adc	a, l
	pop	hl
	ret
