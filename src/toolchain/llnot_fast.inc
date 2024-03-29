	assume	adl=1

	section	.text
	public	__llnot_fast
__llnot_fast: ; CHECK: bitcast(int64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == ~bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) && out.IX == in.IX
	push	hl		; *spl=UHL
	add	hl, de		; uhl=UHL+UDE
	ex	de, hl		; ude=UHL+UDE, uhl=UDE
	scf
	sbc	hl, de		; uhl=UDE-(UHL+UDE)-1
				;    =UDE-UHL-UDE-1
				;    =-UHL-1
				;    =~UHL
	ex	(sp), hl	; uhl=UHL, *spl=~UHL
	scf
	sbc	hl, de		; uhl=UHL-(UHL+UDE)-1
				;    =UHL-UHL-UDE-1
				;    =-UDE-1
				;    =~UDE
	ex	de, hl		; ude=~UDE, uhl=UHL+UDE
	pop	hl		; uhl=~UHL
	ld	a, c		; a=C
	cpl			; a=~C
	ld	c, a		; c=~C
	ld	a, b		; a=B
	cpl			; a=~B
	ld	b, a		; bc=~BC
	ret
