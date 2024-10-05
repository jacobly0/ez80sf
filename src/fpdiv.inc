; IEEE single precision division
; aubc = aubc / euhl
__fpdiv: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), bitcast(float, pair8_24_t, { in.BC, in.A }) / bitcast(float, pair8_24_t, { in.HL, in.E })) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
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
	ld	a, d
	inc	a
	jq	z, .nonfinite.2
	add	a, 080h
	dec	d
	ld	d, a
	ld	a, e
	jq	z, .subnormal.divisor
	jq	nc, .add
	sub	a, d
	ld	e, a
	sbc	a, a
	jq	.continue

.divisor.zero:
	pop	de
	sbc	hl, de
	jq	nz, .return.bc
.return.nan:
	set	7, b
.return.bc:
	pop	af, hl, de
	or	a, 07Fh
	ret

.overflow:
	ld	bc, 0800000h
	jq	.return.bc

.subnormal.divisor:
	ex	(sp), hl
	add	hl, bc
	jq	c, .normalize.divisor.done
	sbc	hl, bc
	jq	z, .divisor.zero
.normalize.divisor.loop:
	dec	d
	add	hl, hl
	add	hl, bc
	jq	nc, .normalize.divisor.loop
.normalize.divisor.done:
	add	hl, bc
	ex	(sp), hl
.add:
	sub	a, d
	ld	e, a
	sbc	a, a
	inc	a
.continue:
	ld	d, a
	pop	bc
	add	hl, bc
	or	a, a
	sbc	hl, bc
	jq	z, .dividend.zero
	or	a, a
.normalize.dividend.loop:
	sbc	hl, bc
	jq	nc, .normalize.dividend.done
	add	hl, bc
	dec	de
	add	hl, hl
	jq	nc, .normalize.dividend.loop
	or	a, a
	sbc	hl, bc
.normalize.dividend.done:
	dec	d
	jq	z, .overflow
	dec	de
	ld	a, e
	dec	de
	inc	d
	ex	de, hl
	ld	hl, 1
	jq	z, .divide.entry
	dec	a
	cp	a, -24
	jq	c, .underflow
.subnormal:
	add	hl, hl
	inc	a
	jq	nz, .subnormal
	inc	a
	jq	nc, .divide.entry
	ex	de, hl
	add	hl, bc
	jq	.subsubnormal

.nonfinite.1:
	inc	d
	pop	de
	jq	z, .return.nan
	pop	af
	or	a, 07Fh
	jq	.return.hl

.nonfinite.2:
	pop	hl
	or	a, a
	sbc	hl, bc
	jq	nz, .return.nan
	inc	hl
.underflow:
	dec	hl
.dividend.zero:
	pop	af
	and	a, 080h
	jq	.return.hl

.divide.loop:
	add	hl, hl
	jq	c, .divide.overflow
	sbc	hl, bc
	jq	nc, .divide.setbit
	add	hl, bc
	ex	de, hl
	add	hl, hl
	ex	de, hl
	jq	nc, .divide.loop
	add	hl, hl
	jq	.divide.finish
.divide.overflow:
	or	a, a
	sbc	hl, bc
.divide.setbit:
	ex	de, hl
.divide.entry:
	add	hl, hl
	inc	hl
	ex	de, hl
	jq	nc, .divide.loop
	add	hl, hl
.subsubnormal:
	inc	hl
.divide.finish:
	ccf
	jq	nc, .round
	sbc	hl, bc
.round:
	sbc	hl, hl
	inc	hl
	add	hl, de
	ld	de, 0800000h
	jq	nc, .rounded
	add	hl, de
	inc	a
.rounded:
	pop	bc
	sla	b
	rra
	jq	c, .return.hl
	add	hl, de
.return.hl:
	ex	(sp), hl
	pop	bc, de
	ret
