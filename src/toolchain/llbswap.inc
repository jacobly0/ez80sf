	assume	adl=1

	section	.text
	public	__llbswap
__llbswap: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == __builtin_bswap64(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS })) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	push	af
	ld	a, b			; a = b
	ld	b, l			; bcudeuhl = lc[deu]de[hlu]hl
	ld	l, d			; bcudeuhl = lc[deu]de[hlu]hd
	ld	d, c			; bcudeuhl = lc[deu]ce[hlu]hd
	ld	c, h			; bcudeuhl = lh[deu]ce[hlu]hd
	ld	h, e			; bcudeuhl = lh[deu]ce[hlu]ed
	ld	e, a			; bcudeuhl = lh[deu]cb[hlu]ed
	ex	de, hl			; bcudeuhl = lh[hlu]ed[deu]cb
	pop	af
	ret
