	assume	adl=1

	section	.text
	public	_toupper
_toupper: ; PREREQ: -1 <= (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 && (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 < 1 << 8 CHECK: (int32_t)(out.HL << 8) >> 8 == (inStack[1][0] >= 'a' && inStack[1][0] <= 'z' ? inStack[1][0] - 'a' + 'A' : (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8) && out.IX == in.IX
	pop	de
	ex	(sp), hl
	push	de

	ld	a, l
	sub	a, 'a'
	cp	a, 1+'z'-'a'
	ret	nc

	add	a, 'A'
	ld	l, a
	ret
