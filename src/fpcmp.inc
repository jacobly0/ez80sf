; IEEE single precision comparison
; z = euhl == aubc
; c = euhl < aubc
; s = !(euhl >= aubc)
__fpcmp: ; CHECK: out.flags.Z == (bitcast(float, pair8_24_t, { in.HL, in.E }) == bitcast(float, pair8_24_t, { in.BC, in.A })) && out.flags.C == (bitcast(float, pair8_24_t, { in.HL, in.E }) < bitcast(float, pair8_24_t, { in.BC, in.A })) && out.flags.S == !(bitcast(float, pair8_24_t, { in.HL, in.E }) >= bitcast(float, pair8_24_t, { in.BC, in.A })) && out.A == in.A && out.BC == in.BC && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	xor	a, e
	jq	z, .maybeEqual
	cp	a, 80h
	jq	c, .signsMatch
	jq	nz, .notBothZero
	xor	a, e
	jq	z, .maybeBothZero
	inc	e
	dec	e
	jq	nz, .notBothZero2
.maybeBothZero:
	adc	hl, bc
	jq	nz, .notBothZeroFixup
	ret	nc	; Both inputs are zero, return Z=1, C=0, S=0
.notBothZeroFixup:
	xor	a, e
	sbc	hl, bc
.notBothZero:
	xor	a, e
.notBothZero2:
	inc	e
	call	z, .checkFirstNan
	call	pe, .checkFirstNan
	dec	e
.secondLargerAbs:
	cp	a, 0FFh
	jq	z, .checkSecondNan
	cp	a, 07Fh
	ret	nz	; C = S = !sign(aubc), Z = 0
.checkSecondNan:
	push	hl
	ld	hl, 07FFFFFh
	add	hl, bc
	pop	hl
	jq	c, .outputNan
	cp	a, 080h	; C = S = !sign(aubc), Z = 0
	ret

.signsMatch:
	xor	a, e
	; Compare upper 7 exponent bits, which are not equal
	cp	a, e
	jq	nc, .secondLargerAbs
	inc	e
	call	z, .checkFirstNan
	call	pe, .checkFirstNan
	dec	e	; S = sign(euhl), Z = 0 because E > A
	rlca
	rrca		; C = sign(aubc)
	ret

.maybeEqual:
	; Sign and upper 7 exponent bits are equal
	ld	a, e
	inc	a
	add	a, a
	ld	a, e
	jq	z, .checkBothNan
	or	a, a
.checkBothNanDone:
	; Compare mantissas and low exponent bit
	sbc	hl, bc
	add	hl, bc
	ret	z	; Both inputs are equal, return Z=1, C=0, S=0
	; XOR the carry with the input sign and place into the output sign/carry
	sbc	a, a
	xor	a, e
	or	a, 07Fh ; Affect S flag, Z = 0
	rlca	; Affect C flag
	ld	a, e
	ret

.checkFirstNan:
	ex	de, hl
	push	hl
	ld	hl, 07FFFFFh
	add	hl, de
	pop	hl
	ex	de, hl
	ret	nc
	dec	e
	inc	sp
	inc	sp
	inc	sp
.outputNan:
	; Carry is always set here
	rr	a ; Z = 0, S = 1
	rla	; Restore A
	ccf	; C = 0
	ret

.checkBothNan:
	ex	de, hl
	push	hl
	ld	hl, 07FFFFFh
	add	hl, de
	jq	c, .gotFirstNan
	sbc	hl, de
	add	hl, bc
.gotFirstNan:
	pop	hl
	ex	de, hl
	jq	nc, .checkBothNanDone
	jq	.outputNan
