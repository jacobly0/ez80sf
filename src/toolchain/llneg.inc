	assume	adl=1

	section	.text
	public	__llneg
__llneg: ; PREREQ: bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) != (int64_t)(UINT64_C(1) << 63) CHECK: bitcast(int64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }) == -bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) && out.A == in.A && out.IX == in.IX && out.IY == in.IY
	or	a, a
	public	__llneg.hijack_nc
.hijack_nc:
	ex	de, hl		; ude=UHL, uhl=UDE
	push	hl		; *spl=UDE
	sbc	hl, hl		; uhl=0
	sbc	hl, de		; uhl=-UHL
	ex	(sp), hl	; uhl=UDE, *spl=-UHL
	ex	de, hl		; ude=UDE, uhl=UHL
	ld	hl, 0		; uhl=0
	sbc	hl, de		; uhl=-UDE-(UHL!=0)
				;    =-UDEUHL>>24
	ex	de, hl		; ude=-UDEUHL>>24, uhl=UDE
	sbc	hl, hl		; uhl=-(UDEUHL!=0)
	or	a, a
	sbc	hl, bc		; uhl=-UBC-(UDEUHL!=0)
				;    =-UBCUDEUHL>>48
	ld	c, l
	ld	b, h		; bc=-BCUDEUHL>>48
	pop	hl		; bcudeuhl=-BCUDEUHL
	ret
