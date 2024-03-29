	assume	adl=1

	section	.text
	public	__brems
__brems: ; PREREQ: in.C && ((int8_t)in.A != (int8_t)(1 << 7) || (int8_t)in.C != -1) CHECK: (int8_t)out.A == (int8_t)in.A % (int8_t)in.C && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; I: A=dividend, C=divisor
; O: a=A%C

	push	hl

	ld	l, a
	push	hl
	call	__bdvrms_abs.hijack_a_l_dividend

	pop	af

	ld	a, h
	pop	hl
	ret	p

	neg
	ret


	extern	__bdvrms_abs.hijack_a_l_dividend
