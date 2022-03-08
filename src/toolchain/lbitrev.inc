	assume	adl=1

	section	.text
	public	__lbitrev
__lbitrev: ; CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == __builtin_bitreverse32(bitcast(uint32_t, pair8_24_t, { in.HL, in.E })) && out.A == in.A && out.BC == in.BC && out.D == in.D && out.IX == in.IX && out.IY == in.IY
	push	af
	call	__internal_bitrev_byte	; a=HLU', UHL=HL0
	ld	l, e			; uhl=HLE
	ld	e, a			; e=HLU'
	call	__internal_bitrev_byte	; a=H', uhl=LE0
	ld	l, a			; uhl=LEH'
	call	__internal_bitrev_byte	; a=L', uhl=EH'0
	ld	l, e			; uhl=EH'HLU'
	ld	e, a			; e=L'
	call	__internal_bitrev_byte	; a=E', uhl=H'HLU'
	ld	l, a			; uhl=H'HLU'E'
	pop	af
	ret

	extern	__internal_bitrev_byte
