	assume	adl=1

	section	.text
	public	__llsub_fast
__llsub_fast: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) - *(uint64_t*)inStack[1]) && out.IX == in.IX
	ld	iy, 0
	add	iy, sp
	push	bc
	ld	bc, (iy + 3)
	sbc	hl, bc
	ex	de, hl
	ld	bc, (iy + 6)
	sbc	hl, bc
	ex	de, hl
	pop	bc
	ld	a, c
	sbc	a, (iy + 9)
	ld	c, a
	ld	a, b
	sbc	a, (iy + 10)
	ld	b, a
	ret
