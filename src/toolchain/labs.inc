	assume	adl=1

	section	.text
	public	_labs
_labs: ; PREREQ: *(int32_t*)inStack[1] != (int32_t)(1U << 31) CHECK: bitcast(int32_t, pair8_24_t, { out.HL, out.E }) == abs(*(int32_t*)inStack[1]) && out.IX == in.IX
	pop	bc
	pop	hl
	pop	de
	push	de
	push	hl
	push	bc
	bit	7, e
	ret	z
	jp	__lneg

	extern	__lneg
