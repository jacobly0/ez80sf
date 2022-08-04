	assume	adl=1

	section	.text
	public	__llcmpu_fast
__llcmpu_fast: ; CHECK: out.flags.Z == (bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) == *(uint64_t*)inStack[1]) && out.flags.C == (bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) < *(uint64_t*)inStack[1]) && out.IX == in.IX
	ld	iy, 0
	add	iy, sp
	ld	a, b
	sub	a, (iy + 10)
	ret	nz
	ld	a, c
	sub	a, (iy + 9)
	ret	nz
	ex	de, hl
	ld	bc, (iy + 6)
	sbc	hl, bc
	ret	nz
	ex	de, hl
	ld	bc, (iy + 3)
	sbc	hl, bc
	ret
