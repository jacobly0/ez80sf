; IEEE single precision absolute value
; aubc = |aubc|
__fpabs: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), fabsf(bitcast(float, pair8_24_t, { in.BC, in.A }))) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	and	a, 07Fh
	ret
