	assume	adl=1

	section	.text
	public	__lldivu
__lldivu: ; PREREQ: *(int64_t*)inStack[1] CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) / *(int64_t*)inStack[1]) && out.A == in.A && out.IX == in.IX && out.IY == in.IY

	push	iy
	push	af

	call	__lldvrmu

	ld	hl, (iy + 15)
	ld	de, (iy + 18)
	ld	bc, (iy + 21)

	pop	af
	pop	iy

	ret

	extern	__lldvrmu
