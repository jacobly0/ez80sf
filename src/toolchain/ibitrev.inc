	assume	adl=1

	section	.text
	public	__ibitrev
__ibitrev: ; CHECK: out.HL == __builtin_bitreverse32(in.HL) >> 8 && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
	push	af
	ld	a, h			; a=H
	ld	h, l
	ld	l, a			; uhl=[HLU]LH
	call	__internal_bitrev_byte	; a=HLU', uhl=LH0
	ld	l, a			; uhl=LH[HLU']
	call	__internal_bitrev_byte	; a=L', uhl=H[HLU']0
	ld	l, h
	ld	h, a			; uhl=HL'[HLU']
	call	__internal_bitrev_byte	; a=H', uhl=L'[HLU']0
	ld	l, h
	ld	h, a			; uhl=L'H'[HLU']
	pop	af
	ret

	extern	__internal_bitrev_byte
