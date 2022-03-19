	assume	adl=1

	section	.text
	public	__lshru
__lshru: ; PREREQ: in.L < 32 CHECK: bitcast(uint32_t, pair8_24_t, { out.BC, out.A }) == bitcast(uint32_t, pair8_24_t, { in.BC, in.A }) >> in.L && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
; CC: if C!=0: C*(15*r(PC)+3*r(SPL)+3*w(SPL)+4)+17*r(PC)+9*r(SPL)+6*w(SPL)+2
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
	srl	a
	jp	__lshrs.hijack_lshru

	extern	__lshrs.hijack_lshru
