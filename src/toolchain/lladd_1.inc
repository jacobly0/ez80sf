	assume	adl=1

	section	.text
	public	__lladd_1
__lladd_1: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) + 1) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	inc	hl
	add	hl, de
	or	a, a
	sbc	hl, de
	ret	nz
	inc	de
	sbc	hl, de
	add	hl, de
	ret	nz
	inc	bc
	ret
