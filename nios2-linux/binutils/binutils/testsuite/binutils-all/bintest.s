	.globl text_symbol
	.text
	# this is needed to get the readelf -s, -S and -r tests to work
	# with nios2 as it has relaxation on by default
	.set norelax
text_symbol:	
static_text_symbol:
	.long	1
	.long	external_symbol
	.globl data_symbol
	.data
data_symbol:
static_data_symbol:
	.long	2
	.comm common_symbol,4
