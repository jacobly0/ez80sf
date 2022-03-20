	assume	adl=1

	section	.text
	public	__bctlz
__bctlz: ; CHECK: out.A == (in.A ? __builtin_clz(in.A) - 24 : 8) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	hl

	scf
	sbc	hl, hl

.loop:
	rla
	inc	hl
	jr	nc, .loop

	ld	a, l
	pop	hl
	ret
