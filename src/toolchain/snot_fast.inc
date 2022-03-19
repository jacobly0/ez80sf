	assume	adl=1

	section	.text
	public	__snot_fast
__snot_fast := __inot_fast ; CHECK: out.HLs == (uint16_t)~in.HLs && out.IX == in.IX

	extern	__inot_fast
