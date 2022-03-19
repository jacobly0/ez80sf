	assume	adl=1

	section	.text
	public	__sneg_fast
__sneg_fast := __ineg_fast ; CHECK: out.HLs == (uint16_t)-in.HLs && out.IX == in.IX

	extern	__ineg_fast
