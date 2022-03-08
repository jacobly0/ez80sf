	assume	adl=1

	section	.text
	public	__lladd_b
__lladd_b: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) + in.A) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	push	bc
	inc.s	bc
	ld	c, a
	ld	b, 0
	add	hl, bc
	jr	nc, .finish
	ld	c, 1
	ex	de, hl
	add	hl, bc
	ex	de, hl
.finish:
	pop	bc
	ret	nc
	inc	bc
	ret
