	assume	adl=1

	section	.text
	public	__llcmpzero
__llcmpzero: ; CHECK: out.flags.Z == !bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) && out.A == in.A && out.BCS == in.BCS && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	inc	b
	dec	b
	ret	nz
	inc.s	bc
	dec	c
	jr	nz, .p_nz
	or	a, a
	sbc	hl, bc
	jr	nz, .p_nz
	sbc	hl, de
	ret	z
	add	hl, de
.p_nz:
	inc	b
	ld	b, 0
	ret
