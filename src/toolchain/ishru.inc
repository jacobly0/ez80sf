	assume	adl=1

	section	.text
	public	__ishru
__ishru: ; PREREQ: in.C < 24 CHECK: out.HL == in.HL >> in.C && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
; CC: if C!=0: C*(13*r(PC)+3*r(SPL)+3*w(SPL)+4)+17*r(PC)+9*r(SPL)+6*w(SPL)+2
;     if C==0: 4*r(PC)+3*r(SPL)+2
	inc	c
	dec	c
	ret	z
	push	bc
	push	hl
	ld	b, c
	or	a, a
	sbc	hl, hl
	add	hl, sp
.loop:
	inc	hl
	inc	hl
	srl	(hl)
	jp	__ishrs.hijack_ishru

	extern	__ishrs.hijack_ishru