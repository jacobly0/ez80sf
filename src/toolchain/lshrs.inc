	assume	adl=1

	section	.text
	public	__lshrs
__lshrs: ; PREREQ: in.L < 32 CHECK: bitcast(int32_t, pair8_24_t, { out.BC, out.A }) == bitcast(int32_t, pair8_24_t, { in.BC, in.A }) >> in.L && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
; CC: if C!=0: C*(15*r(PC)+3*r(SPL)+3*w(SPL)+4)+13*r(PC)+9*r(SPL)+6*w(SPL)+1
;     if C==0: 4*r(PC)+3*r(SPL)+2
	inc	l
	dec	l
	ret	z
	push	hl
	push	bc
	ld	b, l
	or	a, a
	sbc	hl, hl
	add	hl, sp
.loop:
	sra	a
	public	__lshrs.hijack_lshru
.hijack_lshru:
	inc	hl
	inc	hl
	rr	(hl)
	dec	hl
	rr	(hl)
	dec	hl
	rr	(hl)
	djnz	.loop
	pop	bc
	pop	hl
	ret
