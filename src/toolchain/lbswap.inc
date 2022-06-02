	assume	adl=1

	section	.text
	public	__lbswap
__lbswap: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == __builtin_bswap32(bitcast(uint32_t, pair8_24_t, { in.HL, in.E })) && out.A == in.A && out.BC == in.BC && out.D == in.D && out.IX == in.IX && out.IY == in.IY
	push	hl			; ...[hlu]hl <- (sp)
	inc	sp			; ...[hlu]h <- (sp)
	ld	h, e			; uhl = [hlu]el
	push	hl			; ...[hlu]h[hlu]el <- (sp)
	inc	sp			; ...[hlu]h[hlu]e <- (sp)
	ld	e, l
	pop	hl			; euhl = lh[hlu]e
					; ...[hlu] <- (sp)
	inc	sp			; ... <- (sp)
	ret
