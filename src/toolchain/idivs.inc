	assume	adl=1

	section	.text
	public	__idivs
__idivs: ; PREREQ: in.BC && (in.HL != 1 << 23 || in.BC != (1 << 24) - 1) CHECK: ((int32_t)(out.HL << 8) >> 8) == ((int32_t)(in.HL << 8) >> 8) / ((int32_t)(in.BC << 8) >> 8) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
; I: UHL=dividend, UBC=divisor
; O: uhl=UHL/UBC

	push	de

	call	__idvrms

	pop	de
	ret


	extern	__idvrms
