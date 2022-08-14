__fpupop1:
	ld	bc, 0800000h
__fpupop2:
	sla	e
	jq	z, .denormal
	add	hl, bc
	ret	nc
	add	hl, bc
.denormal:
	inc	e
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

__fppack2.normalize:
	add	ix, ix
	adc	hl, hl
__fppack2:
	add	hl, de
	jq	c, .normalized
	djnz	.normalize
	add	hl, de
.normalized:
	ld	a, b
	push	ix
	ex	(sp), hl
	or	a, a
	adc	hl, hl
	pop	hl
	jq	nc, .rounded
	jq	nz, .round
	bit	0, l
	jq	z, .rounded
.round:
	adc	hl, de
	adc	a, e
.clear:
	add	hl, de
	jq	nc, .clear
.rounded:
	cp	a, 0FFh
	jq	z, .infinite
	sla	c
	rra
	ret	nc
	add	hl, de
	ret
.infinite:
	sla	c
	rra
	ex	de, hl
	ret
