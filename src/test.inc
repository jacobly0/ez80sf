return: ; CHECK: true
	ret
loop: ; CHECK: true
	jr	$
busy: ; CHECK: out.B == 0
	ld	b, 0
	djnz	$
	ret
fail: ; CHECK: out.A
	xor	a, a
	ret
