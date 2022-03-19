	assume	adl=1

	section	.text
	public	__bremu
__bremu: ; PREREQ: in.C CHECK: out.A == (uint8_t)(in.A % in.C) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; I: A=dividend, C=divisor
; O: a=A%C

	push	bc
	push	hl

	ld	b, a
	call	__bdvrmu
	ld	a, h

	pop	hl
	pop	bc
	ret


	extern	__bdvrmu
