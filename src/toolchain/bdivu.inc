	assume	adl=1

	section	.text
	public	__bdivu
__bdivu: ; PREREQ: in.C CHECK: out.A == (uint8_t)(in.B / in.C) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; I: B=dividend, C=divisor
; O: a=B/C

	push	bc
	push	hl

	call	__bdvrmu
	ld	a, l

	pop	hl
	pop	bc
	ret


	extern	__bdvrmu
