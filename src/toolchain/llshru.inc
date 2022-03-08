	assume	adl=1

	section	.text
	public	__llshru
__llshru: ; PREREQ: *(uint8_t*)inStack[1] < 64 CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) >> *(uint8_t*)inStack[1]) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
	push	af
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	a, (iy + 9)
	or	a, a
	jp	z, __llshrs.finish
	push	de
	push	hl
.loop:
	srl	b
	jp	__llshrs.hijack_llshru

	extern	__llshrs.finish
	extern	__llshrs.hijack_llshru
