; IEEE single precision negation
; aubc = -aubc
__fpneg: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), -bitcast(float, pair8_24_t, { in.BC, in.A })) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	xor	a, 080h
	ret
