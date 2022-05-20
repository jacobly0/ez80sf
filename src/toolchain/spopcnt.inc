	assume	adl=1

	section	.text
	public	__spopcnt
__spopcnt: ; CHECK: out.A == __builtin_popcount(in.HLs) && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	; Set HLU=H and H=L, while saving HL on the stack
	push	hl
	dec	sp
	ex	(sp), hl
	; Calculate 8-popcount(H)-popcount(HLU)
	ld	a, h
	call	__popcnt_common_init
	; Subtract final value from 8, and accumulate output carry
	cpl
	adc	a, 9
	pop	hl
	inc	sp
	ret

	extern	__popcnt_common_init
