	assume	adl=1

	section	.text
	public	__ishrs_1_fast
__ishrs_1_fast: ; CHECK: (int32_t)(out.HL << 8) >> 8 == (int32_t)(in.HL << 8) >> 8 >> 1 && out.IX == in.IX
	push	hl
	ld	hl, 2
	add	hl, sp
	sra	(hl)
	pop	hl
	rr	h
	rr	l
	ret
