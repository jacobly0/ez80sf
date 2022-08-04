	assume	adl=1

	section	.text
	public	__llcmps
__llcmps: ; CHECK: (out.flags.PV ^ out.flags.S) == (bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) < *(int64_t*)inStack[1]) && out.A == in.A && out.BCS == in.BCS && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	iy
	ld	iy, 0
	add	iy, sp
	push	bc
	push	hl
	ld	l, c
	ld	h, b
	ld	bc, (iy + 12)
	sbc.s	hl, bc
	pop	hl
	jr	nz, .finish
	ld	bc, (iy + 9)
	ex	de, hl
	sbc	hl, bc
	add	hl, bc
	ex	de, hl
	jr	nz, .finish_unsigned
	ld	bc, (iy + 6)
	sbc	hl, bc
	add	hl, bc
.finish_unsigned:
	ld	b, a
	sbc	a, a
	ld	a, b
.finish:
	pop	bc
	pop	iy
	ret
