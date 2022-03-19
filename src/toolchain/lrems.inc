	assume	adl=1

	section	.text
	public	__lrems
__lrems: ; PREREQ: bitcast(int32_t, pair8_24_t, { in.BC, in.A }) && (bitcast(int32_t, pair8_24_t, { in.HL, in.E }) != (int32_t)(1U << 31) || bitcast(int32_t, pair8_24_t, { in.BC, in.A }) != -1) CHECK: bitcast(int32_t, pair8_24_t, { out.HL, out.E }) == (int32_t)(bitcast(int32_t, pair8_24_t, { in.HL, in.E }) % bitcast(int32_t, pair8_24_t, { in.BC, in.A })) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
; I: EUHL=dividend, AUBC=divisor
; O: euhl=EUHL%AUBC

	bit	7, e
	push	af
	push	bc

	call	__ldivs_lrems_common

	call	__lremu

	pop	bc
	pop	af
	ret	z
	jp	__lneg


	extern	__ldivs_lrems_common
	extern	__lneg
	extern	__lremu
