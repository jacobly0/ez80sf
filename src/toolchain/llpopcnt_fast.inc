	assume	adl=1

	section	.text
	public	__llpopcnt_fast
__llpopcnt_fast: ; CHECK: out.A == __builtin_popcountl(bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS })) && out.IX == in.IX
	call	__lpopcnt_fast
	push	de
	push	hl
	ex	de, hl
	ld	l, b
	ld	e, c
	ld	d, a
	call	__lpopcnt_fast
	add	a, d
	pop	hl
	pop	de
	ret

	extern	__lpopcnt_fast
