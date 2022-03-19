	assume	adl=1

	section	.text
	public	__lldivu_b
__lldivu_b: ; PREREQ: *(int8_t*)inStack[1] CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) / *(uint8_t*)inStack[1]) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
; Very slow
	push	af
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	iy, (iy + 9)
	xor	a, a
	ld	iyh, 64
.loop:
	add	hl, hl
	ex	de, hl
	adc	hl, hl
	ex	de, hl
	rl	c
	rl	b
	rla
	jr	c, .1
	cp	a, iyl
	jr	c, .0
.1:
	sub	a, iyl
	inc	l
.0:
	dec	iyh
	jr	nz, .loop
	pop	iy
	pop	af
	ret
