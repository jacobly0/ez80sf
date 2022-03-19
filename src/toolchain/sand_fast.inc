	assume	adl=1

	section	.text
	public	__sand_fast
__sand_fast: ; CHECK: out.HLs == (in.HLs & in.BCS) && out.IX == in.IX
	ld	a, h
	and	a, b
	ld	h, a
	ld	a, l
	and	a, c
	ld	l, a
	ret
