	assume	adl=1

	section	.text
	public	_tolower
_tolower: ; PREREQ: -1 <= (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 && (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8 < 1 << 8 CHECK: (int32_t)(out.HL << 8) >> 8 == (inStack[1][0] >= 'A' && inStack[1][0] <= 'Z' ? inStack[1][0] - 'A' + 'a' : (int32_t)(*(uint32_t*)inStack[1] << 8) >> 8) && out.IX == in.IX
	pop	de
	ex	(sp), hl
	push	de

	ld	a, l
	sub	a, 'A'
	cp	a, 1+'Z'-'A'
	ret	nc

	add	a, 'a'
	ld	l, a
	ret
