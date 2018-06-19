@ logical not (0 becomes 1, anything else becomes 0)
not:
	neg   r1, r0 @ r1 = -r0; C = 1 if r0 is zero
	adc   r0, r1

	@ return
	bx    lr

@ signed divition by 2 that rounds towards zero (rather than down)
signed_div_by_2:
	@ add 1 to r0 if r0 is negative
	lsr   r1, r0, #31
	add   r0, r1

	@ divide by 2
	asr   r0, #1

	@ return
	bx    lr

@ r0 becomes its absolute value
abs:
	asr   r1, r0, #31
	add   r0, r1
	eor   r0, r1

	@ return
	bx    lr
