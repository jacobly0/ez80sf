	assume	adl=1

	section	.text
	public  _abs
_abs: ; CHECK: out.HL == (abs((int32_t)(*(uint32_t*)inStack[1] << 8) >> 8) & 0xFFFFFF) && out.IX == in.IX
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
