	assume	adl=1

	section	.text
	public	__bbitrev
__bbitrev: ; CHECK: out.A == __builtin_bitreverse8(in.A) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	bc
	ld	b,a 		; b=ABCDEFGH
	rrca 			; a=HABCDEFG
	rrca 			; a=GHABCDEF
	xor	a,b
	and	a,$aa
	xor 	a,b 		; a=GBADCFEH
	ld	b,a 		; b=GBADCFEH
	rrca 			; a=HGBADCFE
	rrca 			; a=EHGBADCF
	rrca 			; a=FEHGBADC
	rrca 			; a=CFEHGBAD
	xor	a,b
	and	a,$66
	xor	a,b 		; a=GFEDCBAH
	rrca 			; a=HGFEDCBA
	pop	bc
	ret

