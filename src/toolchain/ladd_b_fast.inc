	assume	adl=1

	section	.text
	public	__ladd_b_fast
__ladd_b_fast: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) + in.A && out.IX == in.IX
	add	a, l
	ld	l, a
	ret	nc
	inc	h
	ret	nz
	ld	bc, 1 shl 16
	add	hl, bc
	ret	nc
	inc	e
	ret
