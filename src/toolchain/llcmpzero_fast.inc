	assume	adl=1

	section	.text
	public	__llcmpzero_fast
__llcmpzero_fast: ; CHECK: out.flags.Z == !bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) && out.IX == in.IX
	xor	a, a
	add	a, b
	ret	nz
	cp	a, c
	jr	nz, .p_nz
	mlt	bc
	adc	hl, bc
	jr	nz, .p_nz
	adc	hl, de
	ret	z
.p_nz:
	inc	a
	ret
