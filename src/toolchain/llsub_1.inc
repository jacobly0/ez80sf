	assume	adl=1

	section	.text
	public	__llsub_1
__llsub_1: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) - 1) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	add	hl, de
	or	a, a
	sbc	hl, de
	dec	hl
	ret	nz
	ex	de, hl
	add	hl, de
	ex	de, hl
	ret	c
	dec	bc
	ret
