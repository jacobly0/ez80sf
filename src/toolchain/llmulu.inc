	assume	adl=1

	section	.text
	public	__llmuls, __llmulu
__llmuls:
__llmulu: ; CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == (uint64_t)(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) * *(int64_t*)inStack[1]) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
; Really slow

	push	ix
	push	iy
	push	af

	ld	ix, 0
	lea	iy, ix - 6
	add	iy, sp		; cf=1

	push	de
	push	hl
	ld	l, c
	ld	h, b
	ld.s	sp, hl

	lea	hl, iy + 18
	ld	b, 8
.push_loop:
	push	af
	ld	a, (hl)
	inc	hl
	or	a, a		; cf=0
	djnz	.push_loop

	sbc	hl, hl
	ld	e, l
	ld	d, h

.byte_loop:
	scf
	adc	a, a

.bit_loop:
	push	af
	add	ix, ix
	adc	hl, hl
	ex	de, hl
	adc	hl, hl
	ex	de, hl
	pop	af

	jr	nc, .add_end
	ld	bc, (iy)
	add	ix, bc
	ld	bc, (iy + 3)
	adc	hl, bc
	ex	de, hl
	adc.s	hl, sp
	ex	de, hl
.add_end:

	add	a, a
	jr	nz, .bit_loop

	pop	af
	jr 	nc, .byte_loop

	ld	b, d
	ld	c, e
	ex	de, hl
	lea	hl, ix

	pop	af
	pop	af
	pop	af
	pop	iy
	pop	ix
	ret
