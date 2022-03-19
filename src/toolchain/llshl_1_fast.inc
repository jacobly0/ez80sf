	assume	adl=1

	section	.text
	public	__llshl_1_fast
__llshl_1_fast: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) << 1 && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	add	hl, hl
	ex	de, hl
	adc	hl, hl
	ex	de, hl
	rl	c
	rl	b
	ret
