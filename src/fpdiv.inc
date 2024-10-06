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
	ld	c, a
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
	ld	c, a
	sbc	a, a
	inc	a
.continue:
	ld	b, a
	pop	de
	ex	de, hl  ; uhl=UDE, ude=UHL
	add	hl, de	; uhl=UHL+UDE
	ex	de, hl	; ude=UHL+UDE, uhl=UHL
	or	a, a
	sbc	hl, de	; uhl=-UDE
	ex	de, hl	; ude=-UDE, uhl=UHL+UDE
	or	a, a
	adc	hl, de	; uhl=UHL
	jq	z, .dividend.zero
.normalize.dividend.loop:
	add	hl, de
	jq	c, .normalize.dividend.done
	sbc	hl, de
	dec	bc
	add	hl, hl
	jq	nc, .normalize.dividend.loop
	add	hl, de
.normalize.dividend.done:
	djnz	.no_overflow
	ld	c, b
	jq	.return.bc
.no_overflow:
	dec	bc
	dec	bc
	ld	a, c
	inc	b
	ld	bc, 0800000h or (23 shl 8)
	jq	z, .divide.entry.normal
	scf
	adc	a, b
	jq	nc, .underflow
	ld	b, a
	ld	a, c
	push	iy
	ld	iy, 0
	jq	nz, .divide.entry.subnormal
	dec	hl
	add	hl, de
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
	dec	hl
	add	hl, bc
	jq	c, .return.nan
.underflow:
	sbc	hl, hl
.dividend.zero:
	pop	af
	and	a, 080h
	jq	.return.hl

.divide.entry.normal:
	push	iy
	ld	iyl, b
.divide.loop:
	add	iy, iy
	add	hl, hl
	jq	c, .divide.overflow
	add	hl, de
	jq	c, .divide.setbit
	sbc	hl, de
	djnz	.divide.loop
	add	hl, hl
	jq	.divide.finish
.divide.overflow:
	add	hl, de
.divide.setbit:
.divide.entry.subnormal:
	inc	iy
	djnz	.divide.loop
	add	hl, hl
	inc	hl
.divide.finish:
	jq	c, .round
	dec	de
	add	hl, de
.round:
	ccf
.subsubnormal:
	lea	de, iy
	pop	iy
	sbc	hl, hl
	inc	hl
	add	hl, de
	pop	de
	ld	e, a
	adc	a, 1
	sla	d
	rra
	srl	e
	jq	nc, .return.hl
	add	hl, bc
.return.hl:
	ex	(sp), hl
	pop	bc, de
	ret
