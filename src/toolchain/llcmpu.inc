	assume	adl=1

	section	.text
	public	__llcmpu
__llcmpu: ; CHECK: out.flags.Z == (bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) == *(uint64_t*)inStack[1]) && out.flags.C == (bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) < *(uint64_t*)inStack[1]) && out.A == in.A && out.BCS == in.BCS && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	iy
	ld	iy, 0
	add	iy, sp
	push	hl
	ld	hl, (iy + 12)
	sbc.s	hl, bc
	jr	nz, .ne
	ld	hl, (iy + 9)
	sbc	hl, de
	jr	nz, .ne
	pop	hl
	push	bc
	ld	bc, (iy + 6)
	sbc	hl, bc
	add	hl, bc
	pop	bc
	pop	iy
	ret

.ne:
	ccf
	pop	hl
	pop	iy
	ret
