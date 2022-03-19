	assume	adl=1

	section	.text
	public	_isspace
_isspace: ; PREREQ: -1 <= (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 && (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 < 1 << 8 CHECK: !!out.HL == (inStack[1][0] == ' ') && out.IX == in.IX
	pop	hl
	pop	de
	push	de
	push	hl
	or	a,a
	sbc	hl,hl
	bit	7,e
	ret	nz
	ex	de,hl
	ld	de,___maptab
	add	hl,de
	ld	a,(hl)
	and	a,64
	sbc	hl,hl
	ld	l,a
	ret

	extern	___maptab
