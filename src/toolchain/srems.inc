	assume	adl=1

	section	.text
	public	__srems
__srems: ; PREREQ: in.BCS && ((int16_t)in.HLs != (int16_t)(1 << 15) || (int16_t)in.BCS != -1) CHECK: (int16_t)out.HLs == (int16_t)in.HLs % (int16_t)in.BCS && out.A == in.A && out.BCS == in.BCS && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
; I: HL=dividend, BC=divisor
; O: uhl=HL%BC

	push	de

	call	__sdvrms_abs

	pop	de
	ret	nc

	jp	__ineg


	extern	__ineg
	extern	__sdvrms_abs
