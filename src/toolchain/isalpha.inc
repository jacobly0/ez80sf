	assume	adl=1

	section	.text
	public	_isalpha
_isalpha: ; PREREQ: -1 <= (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 && (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 < 1 << 8 CHECK: !!out.HL == (('A' <= inStack[1][0] && inStack[1][0] <= 'Z') || ('a' <= inStack[1][0] && inStack[1][0] <= 'z')) && out.IX == in.IX
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
	and	a,12
	sbc	hl,hl
	ld	l,a
	ret

	extern	___maptab