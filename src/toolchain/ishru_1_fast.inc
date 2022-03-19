	assume	adl=1

	section	.text
	public	__ishru_1_fast
__ishru_1_fast: ; CHECK: out.HL == in.HL >> 1 && out.IX == in.IX
	push	hl
	ld	hl, 2
	add	hl, sp
	srl	(hl)
	pop	hl
	rr	h
	rr	l
	ret
