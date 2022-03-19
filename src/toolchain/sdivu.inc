	assume	adl=1

	section	.text
	public	__sdivu
__sdivu: ; PREREQ: in.BCS CHECK: out.HLs == in.HLs / in.BCS && out.A == in.A && out.BCS == in.BCS && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
; I: HL=dividend, BC=divisor
; O: uhl=HL/BC

	push	de

	call	__sdvrmu
	ex	de, hl

	pop	de
	ret


	extern	__sdvrmu
