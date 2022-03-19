	assume	adl=1

	section	.text
	public	__llshl
__llshl: ; PREREQ: *(uint8_t*)inStack[1] < 64 CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) << *(uint8_t*)inStack[1] && out.A == in.A && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
	push	af
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	a, b
	ld	b, (iy + 9)
	inc	b
	dec	b
	jr	z, .finish
.loop:
	add	hl, hl
	ex	de, hl
	adc	hl, hl
	ex	de, hl
	rl	c
	rla
	djnz	.loop
.finish:
	ld	b, a
	pop	iy
	pop	af
	ret
