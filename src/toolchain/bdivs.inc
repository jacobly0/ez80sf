	assume	adl=1

	section	.text
	public	__bdivs
__bdivs: ; PREREQ: in.C && ((int8_t)in.A != -128 || (int8_t)in.C != -1) CHECK: out.A == (uint8_t)((int8_t)in.B / (int8_t)in.C) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; I: B=dividend, C=divisor
; O: a=B/C

	push	hl

	call	__bdvrms

	ld	a, c
	xor	a, b

	ld	a, l
	pop	hl
	ret	p

	neg
	ret


	extern	__bdvrms
