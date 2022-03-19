	assume	adl=1

	section	.text
	public	__llor_fast
__llor_fast: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) | *(uint64_t*)inStack[1]) && out.IX == in.IX
; CC: 56*r(PC)+19*r(SPL)+8*w(SPL)+1
	ld	iy, 0
	add	iy, sp
	push	hl
	push	de
	lea	hl, iy + 10
	ld	a, b
	or	a, (hl)
	ld	b, a
	dec	hl
	ld	a, c
	or	a, (hl)
	ld	c, a
	dec	hl
	lea	de, iy - 4
	ld	a, (de)
	or	a, (hl)
	ld	(de), a
	pop	de
	dec	hl
	ld	a, d
	or	a, (hl)
	ld	d, a
	dec	hl
	ld	a, e
	or	a, (hl)
	ld	e, a
	dec	hl
	ld	a, (iy - 1)
	or	a, (hl)
	ld	(iy - 1), a
	pop	hl
	ld	a, h
	or	a, (iy + 4)
	ld	h, a
	ld	a, l
	or	a, (iy + 3)
	ld	l, a
	ret
