	assume	adl=1

	section	.text
	public	__llshru_fast
__llshru_fast: ; PREREQ: *(uint8_t*)inStack[1] < 64 CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) >> *(uint8_t*)inStack[1] && out.IX == in.IX
	ld	iy, 0
	add	iy, sp

	ld	a, (iy + 3)
	or	a, a
	ret	z

	ld	a, c
	ld	c, b
	ld	b, (iy + 3)

	push	de
	push	hl

	srl	c
	jp	__llshrs_fast.hijack_llshru


	extern	__llshrs_fast.hijack_llshru
