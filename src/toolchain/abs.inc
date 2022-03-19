	assume	adl=1

	section	.text
	public  _abs
_abs: ; PREREQ: (*(uint32_t*)inStack[1] & ((1 << 24) - 1)) != 1 << 23 CHECK: (int32_t)(out.HL << 8) >> 8 == abs((int32_t)(*(uint32_t*)inStack[1] << 8) >> 8) && out.IX == in.IX
	pop	hl
	pop	de
	push	de
	push	hl
	or	a,a
	sbc	hl,hl
	sbc	hl,de
	ret	p
	ex	de,hl
	ret
