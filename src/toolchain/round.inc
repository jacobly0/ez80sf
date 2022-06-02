	assume	adl=1

	section	.text
	public	_roundf
_roundf: ; CHECK: same(bitcast(float, pair8_24_t, { out.HL, out.E }), roundf(*(float*)inStack[1])) && out.IX == in.IX
;    exponent_biased >= 0 && exponent_biased < 256
; && exponent_biased = exponent + 127
;  = exponent + 127 >= 0 && exponent + 127 < 256
;  = exponent >= -127 && exponent < 129

	ld	hl, 6
	add	hl, sp			; uhl = (char*)&arg + 3
	ld	e, (hl)
	dec	hl
	ld	d, (hl)			; d = exponent + 127 << 7 | fraction >> 16
	dec	hl
	dec	hl
	ld	hl, (hl)		; euhl = arg

	ld	a, e			; a = sign << 7 | exponent + 127 >> 1
	inc	a			; a = (sign << 7 | exponent + 127 >> 1) + 1
					;   = (sign << 7) + (exponent + 127 >> 1) + 1
					;   = (sign << 7) + (exponent + 129 >> 1)
	add	a, a			; a = (sign << 7) + (exponent + 129 >> 1) << 1
					;   = (sign << 8) + (exponent + 129 & ~1)
					;   = exponent + 129 & ~1
					; sf = (exponent + 129 & ~1 & 0x80) != 0
					;    = (exponent + 129 & 0x80) != 0
					;    = (exponent + 1 + 0x80 & 0x80) != 0
					;    = (exponent + 1 & 0x80 ^ 0x80) != 0
					;    = (exponent + 1 & 0x80) != 0x80
					;    = (exponent + 1 & 0x80) == 0
					;   && exponent >= -127 && exponent < 129
					;    = exponent >= -1 && exponent < 127
					; zf = (exponent + 129 & ~1) == 0
					;   && exponent >= -127 && exponent < 129
					;    = exponent == 127 || exponent = 128
					; cf = ((sign << 7) + (exponent + 129 >> 1) & 0xFF) << 1 >> 8 != 0
					;    = ((sign << 7) + (exponent + 129 >> 1) & 0xFF) >> 7 != 0
					;    = ((sign << 7) + (exponent + 129 >> 1) >> 7 & 1) != 0
					;    = (sign + (exponent + 129 >> 8) & 1) != 0
					;    = (sign & 1) != 0 ^ (exponent + 129 >> 8 & 1) != 0
					;   && (sign == 0 || sign == 1)
					;   && exponent >= -127 && exponent < 129
					;    = sign != 0 ^ (exponent == 127 || exponent == 128)
	jp	p, .special

; exponent >= -1 && exponent < 127

	sub	a, 129 + 23		; a = (exponent + 129 & ~1) - 129 - 23
					;   = exponent - 23 & ~1
					; cf = (exponent - 23 & ~1 & 0x100) != 0
					;    = (exponent - 23 & 0x100) != 0
					;   && exponent >= -1 && exponent < 127
					;    = exponent < 23
	ret	nc

; exponent >= 1 && exponent < 23

	rrca				; a = (exponent - 23 & ~1) << 7 | (exponent - 23 & ~1) >> 1
					;   = (exponent - 23 & ~1) << 7 | exponent - 23 >> 1
	rrca				; a = (exponent - 23 & ~1) << 6 | exponent - 23 >> 2
					; cf = ((exponent - 23 & ~1) << 7 | exponent - 23 >> 1) & 1 != 0
					;    = ((exponent - 23 & ~1) << 7 & 1 | exponent - 23 >> 1 & 1) != 0
					;    = (exponent - 23 >> 1 & 1) != 0
					;    = (exponent - 23 & 1 << 1) != 0
	jr	nc, .shift_bit_1_unset

	rrca				; a = (exponent - 23 & ~1) << 5 | exponent - 23 >> 3
					; cf = ((exponent - 23 & ~1) << 6 | exponent - 23 >> 2) & 1 != 0
					;    = ((exponent - 23 & ~1) << 6 & 1 | exponent - 23 >> 2 & 1) != 0
					;    = (exponent - 23 >> 2 & 1) != 0
					;    = (exponent - 23 & 1 << 2) != 0
	ld	bc, 1 shl (8 + 8 - 1 - 2)
	jr	nc, .shift_bits_2_1_done
	ld	b, 1 shl (8 - 1 - 2 - 4)
	jr	.shift_bits_2_1_done


.shift_bit_1_unset:
	rrca				; a = (exponent - 23 & ~1) << 5 | exponent - 23 >> 3
					; cf = ((exponent - 23 & ~1) << 6 | exponent - 23 >> 2) & 1 != 0
					;    = ((exponent - 23 & ~1) << 6 & 1 | exponent - 23 >> 2 & 1) != 0
					;    = (exponent - 23 >> 2 & 1) != 0
					;    = (exponent - 23 & 1 << 2) != 0
	ld	bc, 1 shl (8 + 8 - 1)
	jr	nc, .shift_bits_2_1_done
	ld	b, 1 shl (8 - 1 - 4)
.shift_bits_2_1_done:			; ubc = 1 << 8 + 8 - 1 - 2 * ((exponent - 23 & 1 << 1) != 0) - 4 * ((exponent - 23 & 1 << 2) != 0)
					;     = 1 << 15 - (exponent - 23 & 1 << 1) - (exponent - 23 & 1 << 2)
					;     = 1 << 15 - (exponent - 23 & (1 << 2 | 1 << 1))

	bit	7, d			; zf = (exponent + 127 & 1) == 0
					;    = (exponent - 23 & 1) == 0
	jr	z, .shift_done
	rrc	b
.shift_done:				; ubc = 1 << 15 - (exponent - 23 & (1 << 2 | 1 << 1)) >> !((exponent - 23 & 1) == 0)
					;     = 1 << 15 - (exponent - 23 & (1 << 2 | 1 << 1)) >> (exponent - 23 & 1)
					;     = 1 << 15 - (exponent - 23 & (1 << 2 | 1 << 1)) - (exponent - 23 & 1)
					;     = 1 << 15 - (exponent - 23 & (1 << 2 | 1 << 1 | 1))
					;     = 1 << 15 - (exponent - 23 & 7)

	rrca				; a = (exponent - 23 & ~1) << 4 | exponent - 23 >> 4
					; cf = ((exponent - 23 & ~1) << 5 | exponent - 23 >> 3) & 1 != 0
					;    = ((exponent - 23 & ~1) << 5 & 1 | exponent - 23 >> 3 & 1) != 0
					;    = (exponent - 23 >> 3 & 1) != 0
					;    = (exponent - 23 & 1 << 3) != 0
					;   && exponent >= 1 && exponent < 23
					;    = (exponent >= -1 && exponent < 7) || (exponent >= 15 && exponent < 23)
	jr	nc, .round_2_bytes

; (exponent >= -1 && exponent < 7) || (exponent >= 15 && exponent < 23)

	rrca				; a = (exponent - 23 & ~1) << 3 | exponent - 23 >> 5
					; cf = ((exponent - 23 & ~1) << 4 | exponent - 23 >> 4) & 1 != 0
					;    = ((exponent - 23 & ~1) << 4 & 1 | exponent - 23 >> 4 & 1) != 0
					;    = (exponent - 23 >> 4 & 1) != 0
					;    = (exponent - 23 & 1 << 4) != 0
					;   && (exponent >= -1 && exponent < 7) || (exponent >= 15 && exponent < 23)
					;    = exponent >= 15 && exponent < 23
	jr	c, .round_1_byte

.round_3_bytes:
; exponent >= -1 && exponent < 7
; b = 1 << 7 - (exponent - 23 & 7)
;  && exponent >= -1 && exponent < 7
;   = 1 << 7 - (exponent + 1)
;   = 1 << 6 - exponent

	ld	a, d			; ea = arg >> 16
	add	a, b
	jr	nc, .round_3_bytes_add_done
	inc	e
.round_3_bytes_add_done:
	ld	h, a			; eh = (arg >> 16) + (1 << 6 - exponent)

	xor	a, a
	sub	a, b			; a = -(1 << 6 - exponent)
					;   = -1 << 6 - exponent
	add	a, a			; a = -1 << 6 - exponent << 1
					;   = -1 << 7 - exponent
	or	a, 1 shl 7		; a = -1 << 7 - exponent | 1 << 7
					;  && exponent >= -1 && exponent < 7
					;   = exponent == -1 ? -1 << 8 | 1 << 7 : -1 << 7 - exponent | 1 << 7
					;   = exponent == -1 ? -1 << 7 : -1 << 7 - exponent
					;   = -1 << 7 - (exponent == -1) - exponent
	and	a, h			; a = (arg >> 16) + (1 << 6 - exponent) & -1 << 7 - (exponent == -1) - exponent
	push	af
	dec	sp
	pop	hl			; e[hlu] = (arg >> 16) + (1 << 6 - exponent) & -1 << 7 - (exponent == -1) - exponent
	inc	sp

	ld	h, c
	ld	l, c			; euhl = arg + (1 << 22 - exponent) & -1 << 23 - (exponent == -1) - exponent

	ret


.round_2_bytes:
; exponent >= 7 && exponent < 15
; b = 1 << 7 - (exponent - 23 & 7)
;  && exponent >= 7 && exponent < 15
;   = 1 << 7 - (exponent - 7)
;   = 1 << 14 - exponent
; ubc = 1 << 22 - exponent

	xor	a, a
	add	hl, bc
	adc	a, e
	ld	e, a			; euhl = arg + (1 << 22 - exponent)

	xor	a, a
	sub	a, b			; a = -(1 << 14 - exponent)
					;   = -1 << 14 - exponent
	add	a, a			; a = -1 << 14 - exponent << 1
					;   = -1 << 15 - exponent
	and	a, h			; a = arg >> 8 & -1 << 15 - exponent
	ld	h, a
	ld	l, c			; euhl = arg + (1 << 22 - exponent) & -1 << 23 - exponent

	ret


.round_1_byte:
; exponent >= 15 && exponent < 23
; b = 1 << 7 - (exponent - 23 & 7)
;  && exponent >= 15 && exponent < 23
;   = 1 << 7 - (exponent - 15)
;   = 1 << 22 - exponent

	xor	a, a
	ld	c, b
	ld	b, a			; ubc = 1 << 22 - exponent

	add	hl, bc
	adc	a, e
	ld	e, a			; euhl = arg + (1 << 22 - exponent)

	xor	a, a
	sub	a, c			; a = -(1 << 22 - exponent)
					;   = -1 << 22 - exponent
	add	a, a			; a = -1 << 22 - exponent << 1
					;   = -1 << 23 - exponent
	and	a, l			; a = arg & -1 << 23 - exponent
	ld	l, a			; euhl = arg + (1 << 22 - exponent) & -1 << 23 - exponent

	ret


.special:
; (exponent >= -127 && exponent < -1) || exponent == 127 || exponent = 128
; zf = exponent == 127 || exponent = 128

	ret	z

; exponent >= -127 && exponent < -1
; cf = sign

	ld	e, 0
	rr	e
	sbc	hl, hl			; euhl = sign << 31

	ret
