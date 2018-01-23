; IEEE single precision subtraction
; aubc = aubc - euhl
__fpsub: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), bitcast(float, pair8_24_t, { in.BC, in.A }) - bitcast(float, pair8_24_t, { in.HL, in.E })) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	de
	rl	e
	ccf
	rr	e
;	jq	__fpadd.enter
	virtual
		ld	d, 0
		load .ld_d : byte from $$
	end virtual
	db	.ld_d

; IEEE single precision addition
; aubc = aubc + euhl
__fpadd: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), bitcast(float, pair8_24_t, { in.BC, in.A }) + bitcast(float, pair8_24_t, { in.HL, in.E })) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	de
.enter:
	push	hl
	push	bc
	xor	a, e
	ld	d, a
	push	de
	push	bc
	xor	a, e
	call	__fpupop1
	ex	(sp), hl
	ld	d, e
	ld	e, a
	call	__fpupop2
	inc	e
	jq	z, .nonfinite1
	inc	d
	jq	z, .return2
	dec	e
	dec	d
	ld	a, d
	sub	a, e
	jq	z, .rounded
	jq	nc, .sorted
	inc	c
	ex	(sp), hl
	ld	a, e
	sub	a, d
	ld	d, e
.sorted:
	cp	a, 26
	jq	nc, .largest
	push	hl
	ld	b, a
	xor	a, a
	sbc	hl, hl
	add	hl, sp
.shift:
	inc	hl
	inc	hl
	srl	(hl)
	dec	hl
	rr	(hl)
	dec	hl
	rr	(hl)
	rra
	jq	nc, .flushed1
	or	a, b
.flushed1:
	djnz	.shift
	pop	hl
.rounded:
	ld	b, d
	pop	de
	ex	(sp), hl
	bit	7, h
	jq	nz, .willsubtract
	ld	c, 0
.willsubtract:
	bit	7, l
	jq	z, .positive
	inc	c
.positive:
	bit	7, h
	pop	hl
	jq	nz, .subtract
	add	hl, de
	jq	nc, .done
	push	hl
	ld	hl, 2
	add	hl, sp
	scf
	rr	(hl)
	pop	hl
	rr	h
	rr	l
	rra
	jq	nc, .flushed2
	or	a, 1
.flushed2:
	inc	b
	inc	b
	jq	z, .infinite
	dec	b
	jq	.done
.borrow:
	add	hl, de
	ex	de, hl
	neg
.subtract:
	inc	c
	sbc	hl, de
	jq	c, .borrow
.done:
	ld	de, 0800000h
	call	__fppack
	pop	bc
	ex	(sp), hl
	pop	bc
	pop	de
	ret
.nonfinite1:
	inc	d
	jq	z, .nonfinite
.return1:
	pop	bc
.return1.pop1:
	pop	de
	ld	a, d
.return1.pop2:
	xor	a, e
	pop	bc
.return:
	pop	hl
	pop	de
	ret
.largest:
	ld	a, d
	cp	a, e
	jq	z, .return1
.return2:
	pop	bc
.return2.pop1:
	pop	de
.return2.pop2:
	ld	a, e
	pop	bc
	pop	bc
	push	bc
	jq	.return
.infinite:
	ld	a, c
	pop	bc
	pop	hl
	pop	de
	rrca
	or	a, 07Fh
	ld	bc, 0800000h
	ret
.nonfinite:
	xor	a, a
	sbc	hl, bc
	jq	nz, .return1
	pop	hl
;	or	a, a
	sbc	hl, bc
	jq	nz, .return2.pop1
	pop	de
	bit	7, d
	jq	z, .return1.pop2
	pop	bc
	pop	hl
	pop	de
	ld	a, 07Fh
	ld	bc, 0C00000h
	ret

__fpupop1:
	ld	bc, 0800000h
__fpupop2:
	add	hl, bc
	rl	e
	add	hl, bc
	jq	nz, .notdenormal
	inc	e
	ccf
.notdenormal:
	ret	nc
	add	hl, bc
	ret

__fppack.normalize:
	add	a, a
	adc	hl, hl
__fppack:
	add	hl, de
	jq	c, .normalized
	djnz	.normalize
	add	hl, de
.normalized:
	add	a, a
	ld	a, b
	jq	nc, .rounded
	jq	nz, .round
	bit	0, l
	jq	z, .rounded
.round:
	adc	hl, de
	adc	a, e
	cp	a, 0FFh
	jq	z, .infinite
.clear:
	add	hl, de
	jq	nc, .clear
.rounded:
	srl	c
	rra
	ret	nc
	add	hl, de
	ret
.infinite:
	srl	c
	rra
	ex	de, hl
	ret
