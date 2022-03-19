	assume	adl=1

	section	.text
	public	__bshl
__bshl: ; PREREQ: in.B < 8 CHECK: out.A == (uint8_t)(in.A << in.B) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
; CC: if B!=0: B*(4*r(PC)+1)+6*r(PC)+(ADL?6*r(SPL)+3*w(SPL):4*r(SPS)+2*w(SPS))+1
;     if B==0: 4*r(PC)+(ADL?3*r(SPL):2*r(SPS))+2
	inc	b
	dec	b
	ret	z
	push	bc
.loop:
	add	a, a
	djnz	.loop
	pop	bc
	ret
