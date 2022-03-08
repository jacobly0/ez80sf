	assume	adl=1

	section	.text
	public	__sbitrev
__sbitrev: ; CHECK: out.HLs == __builtin_bitreverse16(in.HLs) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
	push	af
	push	hl
	dec	sp
	pop	hl
	inc	sp			; uhl=HL?
	call	__internal_bitrev_byte	; a=H', uhl=L?0
	ld	l, a			; uhl=L?H'
	call	__internal_bitrev_byte	; a=L', hl=H'0
	ld	l, h
	ld	h, a			; hl=L'H'
	pop	af
	ret

	extern	__internal_bitrev_byte
