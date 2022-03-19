	assume	adl=1

	section	.text
	public	__ldivu
__ldivu: ; PREREQ: bitcast(uint32_t, pair8_24_t, { in.BC, in.A }) CHECK: bitcast(uint32_t, pair8_24_t, { out.HL, out.E }) == (uint32_t)(bitcast(uint32_t, pair8_24_t, { in.HL, in.E }) / bitcast(uint32_t, pair8_24_t, { in.BC, in.A })) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
; I: EUHL=dividend, AUBC=divisor
; O: euhl=EUHL/AUBC

if 1
	push	bc

	call	__ldvrmu

	ld	a, b
	pop	bc

	ret	z
	ei
	ret

else
	push	ix
	push	iy

	call	__ldvrmu
	ld	a, iyh
	pop	iy
	ex	(sp), ix
	pop	hl

	ret
end if


	extern	__ldvrmu
