; IEEE single precision multiplication
; aubc = aubc * euhl
__fpmul: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), bitcast(float, pair8_24_t, { in.BC, in.A }) * bitcast(float, pair8_24_t, { in.HL, in.E })) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	de, hl
	xor	a, e
	push	af
	xor	a, e
	push	bc
	call	__fpupop1
	ex	(sp), hl
	ld	d, e
	ld	e, a
	call	__fpupop2
	inc	e
	jq	z, .nonfinite.1
	inc	d
	jq	z, .nonfinite.2
	ld	a, d
	ld	d, b; 0
	rlc	e
	ccf
	rr	e
	jq	nc, .subtract
	add	a, e
	jq	nc, .continue
.overflow:
	pop	af, af, hl, de
	or	a, 07Fh
	ret
.subtract:
	add	a, e
	jq	z, .subnormal
	jq	c, .continue
	cp	a, -23
	jq	c, .underflow
.subnormal:
	dec	a
	ld	d, a
.continue:
	push	hl, ix
	ld	ix, -7
	add	ix, sp
	ld	sp, ix
	ld	c, l
	ld	h, (ix + 13)
	mlt	hl
	ld	(ix + 0), l
	ld	l, h
	ld	h, 0
	ld	b, (ix + 14)
	mlt	bc
	add	hl, bc
	ld	c, (ix + 11)
	ld	b, (ix + 13)
	mlt	bc
	add.s	hl, bc
	ld	(ix + 1), l
	ld	l, h
	ld	h, 0
	rl	h
	ld	c, (ix + 10)
	ld	b, (ix + 15)
	mlt	bc
	add	hl, bc
	ld	c, (ix + 11)
	ld	b, (ix + 14)
	mlt	bc
	add	hl, bc
	ld	bc, (ix + 12)
	mlt	bc
	add	hl, bc
	ld	(ix + 2), hl
	ld	hl, (ix + 3)
	inc	hl
	dec.s	hl
	ld	c, (ix + 11)
	ld	b, (ix + 15)
	mlt	bc
	add	hl, bc
	ld	c, (ix + 12)
	ld	b, (ix + 14)
	mlt	bc
	add	hl, bc
	ld	(ix + 3), hl
	ld	hl, (ix + 4)
	inc	hl
	dec.s	hl
	ld	c, (ix + 12)
	ld	b, (ix + 15)
	mlt	bc
	add	hl, bc
	ld	(ix + 4), hl
	ld	c, (ix + 17)
	cp	a, d
	jq	nz, .normalized
.normalize:
	lea	hl, ix + 5
	srl	(hl)
repeat 5
	dec	hl
	rr	(hl)
end repeat
	jr	nc, .flushed
	ld	(hl), a
.flushed:
	inc	a
	jq	nz, .normalize
.normalized:
	pop	ix, hl
	inc	sp
	ld	b, a
	ld	de, 0800000h
	add	a, -1
	call	nc, __fppack2.normalized
	call	c, __fppack2
	pop	ix, bc, bc, bc
	ex	(sp), hl
	pop	bc, de
	ret
.underflow:
	pop	af, af, hl, de
	and	a, 080h
	ld	bc, 0
	ret
.nonfinite:
	sbc	hl, bc
	jq	z, .return.2
	add	hl, bc
	pop	bc
	jq	.return.1
.nonfinite.1:
	inc	d
	jq	z, .nonfinite
	ex	de, hl
	pop	hl
	add	hl, bc
	or	a, a
	sbc	hl, bc
	ex	de, hl
	jq	nz, .return.1
	ld	h, a
.return.1:
	pop	af
	ex	(sp), hl
	pop	bc
	jq	.return
.nonfinite.2:
	add	hl, bc
	or	a, a
	sbc	hl, bc
	jq	z, .return.nan
.return.2:
	pop	bc, af, bc
	push	bc
.return.pop:
	pop	hl
.return:
	pop	de
	or	a, 07Fh
	ret
.return.nan:
	pop	bc
	set	7, b
	pop	af
	jq	.return.pop
