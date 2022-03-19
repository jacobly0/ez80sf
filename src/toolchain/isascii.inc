	assume	adl=1

	section	.text
	public	_isascii
_isascii: ; PREREQ: -1 <= (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 && (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 < 1 << 8 CHECK: !!out.HL == (inStack[1][0] < 0x80) && out.IX == in.IX
	pop	de
	pop	hl
	push	hl
	push	de

	ld	de,-128
	add	hl,de
	sbc	hl,hl
	inc	hl
	ret
