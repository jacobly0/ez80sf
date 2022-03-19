	assume	adl=1

	section	.text
	public	__llshl
__llshl: ; PREREQ: *(uint8_t*)inStack[1] < 64 CHECK: bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) << *(uint8_t*)inStack[1] && out.A == in.A && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
	push	af
	ld	a, b

	push	hl
	ld	hl, 9
	add	hl, sp
	ld	b, (hl)
	pop	hl

	inc	b
	jr	.begin

.loop:
	add	hl, hl
	ex	de, hl
	adc	hl, hl
	ex	de, hl
	rl	c
	rla
.begin:
	djnz	.loop

	ld	b, a
	pop	af
	ret
