        assume  adl=1

	section	.text
	public	__frameset0
__frameset0:
	pop	hl
	push	ix
	ld	ix,0
	add	ix,sp
	jp	(hl)
