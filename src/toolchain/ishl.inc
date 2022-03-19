	assume	adl=1

	section	.text
	public	__ishl
__ishl: ; PREREQ: in.C < 24 CHECK: out.HL == (in.HL << in.C & 0xFFFFFF) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
; CC: if C!=0: C*(4*r(PC)+1)+7*r(PC)+(ADL?6*r(SPL)+3*w(SPL):4*r(SPS)+2*w(SPS))+1
;     if C==0: 4*r(PC)+(ADL?3*r(SPL):2*r(SPS))+2
	inc	c
	dec	c
	ret	z
	push	bc
	ld	b, c
.loop:
	add	hl, hl
	djnz	.loop
	pop	bc
	ret
