	assume	adl=1

	section	.text
	public	__sctlz
__sctlz: ; CHECK: out.A == (in.HLs ? __builtin_clz(in.HLs) - 16 : 16) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	hl

	scf
	sbc	a, a

.loop:
	adc.s	hl, hl
	inc	a
	jr	nc, .loop

	pop	hl
	ret
