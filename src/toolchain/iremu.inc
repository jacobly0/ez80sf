	assume	adl=1

	section	.text
	public	__iremu
__iremu: ; PREREQ: in.BC CHECK: out.HL == in.HL % in.BC && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
; I: UHL=dividend, UBC=divisor
; O: uhl=UHL%UBC

	push	de

	call	__idvrmu

	pop	de
	ret


	extern	__idvrmu
