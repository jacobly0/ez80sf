return: ; CHECK: 
	ret
loop: ; CHECK: 
	jr	$
busy: ; CHECK: 
	ld	b, 0
	djnz	$
	ret
