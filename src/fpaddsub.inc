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
	add.s	hl, hl
	jq	nc, .add
	ld	l, h
	add	hl, bc
	ld	c, l
	pop	hl
	jq	.subtract

.add:
	ld	c, h
	pop	hl
	add	hl, de
	jq	nc, .done
	ex	de, hl
	sbc	hl, hl
	add	hl, sp
	push	de
	rr	(hl)
	pop	hl
	rr	h
	rr	l
	rra
	jq	nc, .flushed2
	or	a, 1
.flushed2:
	inc	b
	jq	z, .infinite
	djnz	.done2	;always taken

.borrow:
	inc	c
	add	hl, de
.subtract:
	ex	de, hl
	neg
	sbc	hl, de
	jq	c, .borrow
.done:
	dec	b
.done2:
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

.nonfinite:
	xor	a, a
	sbc	hl, bc
	jq	nz, .return1
	pop	hl
	sbc	hl, bc
	jq	nz, .return2.pop1
	pop	de
	bit	7, d
	jq	z, .return1.pop2
	ld	bc, 0C00000h
.infinite:
	ld	a, c
	ld	c, b ;0, also note BCU=080h from __fpupop1 or 0C0h from .nonfinite
	rrca
	or	a, 07Fh
	pop	hl
	pop	hl
	pop	de
	ret
