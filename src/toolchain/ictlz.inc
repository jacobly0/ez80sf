	assume	adl=1

	section	.text
	public	__ictlz
__ictlz: ; CHECK: out.A == (in.HL ? __builtin_clz(in.HL) - 8 : 24) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	hl

	scf
	sbc	a, a

.loop:
	adc	hl, hl
	inc	a
	jr	nc, .loop

	pop	hl
	ret
