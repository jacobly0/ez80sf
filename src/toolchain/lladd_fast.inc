	assume	adl=1

	section	.text
	public	__lladd_fast
__lladd_fast: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) + *(uint64_t*)inStack[1]) && out.IX == in.IX
	ld	iy, 0
	add	iy, sp
	push	bc
	ld	bc, (iy + 3)
	add	hl, bc
	ex	de, hl
	ld	bc, (iy + 6)
	adc	hl, bc
	ex	de, hl
	pop	bc
	ld	a, c
	adc	a, (iy + 9)
	ld	c, a
	ld	a, b
	adc	a, (iy + 10)
	ld	b, a
	ret
