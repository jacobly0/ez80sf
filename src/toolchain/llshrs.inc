	assume	adl=1

	section	.text
	public	__llshrs
__llshrs: ; PREREQ: *(uint8_t*)inStack[1] < 64 CHECK: bitcast(int64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) >> *(uint8_t*)inStack[1] && out.A == in.A && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
	push	af
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	a, (iy + 9)
	or	a, a
	jr	z, .finish
	push	de
	push	hl
.loop:
	sra	b
	public	__llshrs.hijack_llshru
.hijack_llshru:
	rr	c
	rr	(iy - 1)
	rr	(iy - 2)
	rr	(iy - 3)
	rr	(iy - 4)
	rr	(iy - 5)
	rr	(iy - 6)
	dec	a
	jr	nz, .loop
	pop	hl
	pop	de
	public	__llshrs.finish
.finish:
	pop	iy
	pop	af
	ret
