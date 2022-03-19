	assume	adl=1

	section	.text
	public	__sxor_fast
__sxor_fast: ; CHECK: out.HLs == (in.HLs ^ in.BCS) && out.IX == in.IX
	ld	a, h
	xor	a, b
	ld	h, a
	ld	a, l
	xor	a, c
	ld	l, a
	ret
