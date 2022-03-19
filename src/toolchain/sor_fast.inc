	assume	adl=1

	section	.text
	public	__sor_fast
__sor_fast: ; CHECK: out.HLs == (in.HLs | in.BCS) && out.IX == in.IX
	ld	a, h
	or	a, b
	ld	h, a
	ld	a, l
	or	a, c
	ld	l, a
	ret
