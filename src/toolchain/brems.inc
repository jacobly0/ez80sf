	assume	adl=1

	section	.text
	public	__brems
__brems: ; PREREQ: in.C && ((int8_t)in.A != -128 || (int8_t)in.C != -1) CHECK: out.A == (uint8_t)((int8_t)in.A % (int8_t)in.C) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; I: A=dividend, C=divisor
; O: a=A%C

	push	hl

	ld	b, a
	call	__bdvrms
	ld	a, h

	bit	7, b

	pop	hl
	ret	z

	neg
	ret


	extern	__bdvrms
