	assume	adl=1

	section	.text
	public	__llcmps_fast
__llcmps_fast: ; CHECK: (out.flags.PV ^ out.flags.S) == (bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) < *(int64_t*)inStack[1]) && out.IX == in.IX
	ld	iy, 0
	add	iy, sp
	ld	a, b
	sub	a, (iy + 10)
	ret	nz
	ld	a, c
	sub	a, (iy + 9)
	jr	nz, .finish
	ex	de, hl
	ld	bc, (iy + 6)
	sbc	hl, bc
	jr	nz, .finish
	ex	de, hl
	ld	bc, (iy + 3)
	sbc	hl, bc
.finish:
	sbc	a, a
	ret
