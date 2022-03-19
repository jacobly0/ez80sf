	section	.text,"ax",@progbits
	assume	adl = 1
	section	.rodata,"a",@progbits
	public	___maptab
___maptab:
	db	"€`"

	ident	"clang version 14.0.0 (https://github.com/jacobly0/llvm-project 6f20e4eae6d5ddf09bbb75d3ec422f0832d41ada)"
