	assume	adl=1

	section	.text
	public	_imaxabs, _llabs
_imaxabs:
_llabs: ; PREREQ: *(int64_t*)inStack[1] != (int64_t)(1UL << 63) CHECK: bitcast(int64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == labs(*(int64_t*)inStack[1]) && out.IX == in.IX
	pop	iy
	pop	hl
	pop	de
	pop	bc
	push	bc
	push	de
	push	hl
	ld	a, b
	or	a, a
	call	m, __llneg.hijack_nc
	jp	(iy)


	extern	__llneg.hijack_nc
