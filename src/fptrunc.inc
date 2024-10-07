; IEEE single precision truncation
; aubc = trunc(aubc)
__fptrunc: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), truncf(bitcast(float, pair8_24_t, { in.BC, in.A }))) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	bc
	ex	(sp), hl
	add	hl, hl
	ld	h, a
	rla
	sub	a, 07Fh
	jq	c, .return.zero
	sub	a, 24
	jq	nc, .return
	ld	l, c
	cpl
	ld	c, a
	ld	a, l
	push	hl
	sbc	hl, hl
	call	__ishl
	ld	c, a
	call	__iand
	ex	(sp), hl
	pop	bc
.return:
	ld	a, h
	pop	hl
	ret

.return.zero:
	ld	bc, 0
	ld	a, h
	and	a, 080h
	pop	hl
	ret
