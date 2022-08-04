	assume	adl=1

	section	.text
	public	__lcmps

if PREFER_OS_CRT

__lcmps := 0001A8h

else

__lcmps: ; CHECK: (out.flags.PV ^ out.flags.S) == (bitcast(int32_t, pair8_24_t, { in.HL, in.E }) < bitcast(int32_t, pair8_24_t, { in.BC, in.A })) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	de
	ld	d, a
	ld	a, e
	sub	a, d
	jr	nz, .finish
	sbc	hl, bc
	add	hl, bc
	sbc	a, a
.finish:
	ld	a, d
	pop	de
	ret

end if
