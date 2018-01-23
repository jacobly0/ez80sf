; IEEE single precision copy sign
; aubc = copysign(aubc, euhl)
__fpcopysign: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), copysignf(bitcast(float, pair8_24_t, { in.BC, in.A }), bitcast(float, pair8_24_t, { in.HL, in.E }))) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	xor	a, e
	and	a, 07Fh
	xor	a, e
;	jq	__fpcopy

; IEEE single precision copy
; aubc = copy(aubc)
__fpcopy: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), bitcast(float, pair8_24_t, { in.BC, in.A })) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	ret
