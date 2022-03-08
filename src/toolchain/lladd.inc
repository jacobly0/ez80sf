	assume	adl=1

	section	.text
	public	__lladd
__lladd: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) + *(uint64_t*)inStack[1]) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	push	iy
	ld	iy, 0
	add	iy, sp
	push	bc
	ld	bc, (iy + 6)
	add	hl, bc
	ex	de, hl
	ld	bc, (iy + 9)
	adc	hl, bc
	ex	de, hl
	pop	bc
	jr	nc, .nc48
	inc	bc
.nc48:
	ld	iy, (iy + 12)
	add	iy, bc
	lea	bc, iy
	pop	iy
	ret
