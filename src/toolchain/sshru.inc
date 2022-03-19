	assume	adl=1

	section	.text
	public	__sshru
__sshru: ; PREREQ: in.C < 16 CHECK: out.HLs == in.HLs >> in.C && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.IX == in.IX && out.IY == in.IY
; Suboptimal for large shift amounts
; CC: if C!=0: C*(7*r(PC)+1)+7*r(PC)+(ADL?6*r(SPL)+3*w(SPL):4*r(SPS)+2*w(SPS))+1
;     if C==0: 4*r(PC)+(ADL?3*r(SPL):2*r(SPS))+2
	inc	c
	dec	c
	ret	z
	push	bc
	ld	b, c
.loop:
	srl	h
	rr	l
	djnz	.loop
	pop	bc
	ret
