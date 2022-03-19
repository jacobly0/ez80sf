	assume	adl=1

	section	.text
	public	__lshl
__lshl: ; PREREQ: in.L < 32 CHECK: bitcast(uint32_t, pair8_24_t, { out.BC, out.A }) == bitcast(uint32_t, pair8_24_t, { in.BC, in.A }) << in.L && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
; CC: if L!=0: L*(5*r(PC)+1)+9*r(PC)+(ADL?12*r(SPL)+9*w(SPL):8*r(SPS)+6*w(SPS))+1
;     if L==0: 4*r(PC)+(ADL?3*r(SPL):2*r(SPS))+2
	inc	l
	dec	l
	ret	z
	push	bc
	ld	b, l
	ex	(sp), hl
.loop:
	add	hl, hl
	rla
	djnz	.loop
	ex	(sp), hl
	pop	bc
	ret
