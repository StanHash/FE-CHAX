
	.arm

	.global TmApplyTsa
	.type   TmApplyTsa, function

TmApplyTsa:
	@ arg r0 = target tilemap
	@ arg r1 = source "TSA", see below
	@ arg r2 = tile base (added to each tile entry)

	push  {r4-r7}

	ldrb  r3, [r1, #0]   @ var r3 = WIDTH-1
	ldrb  r4, [r1, #1]   @ var r4 = HEIGHT-1

	add   r1, #2         @ increment source ptr by 2 (past header)

	lsl   r7, r4, #6     @ get (HEIGHT-1)*0x20, the tilemap offset of the last row
	add   r0, r7         @ move the target ptr to the last row

	mov   r6, r4         @ var r6 = HEIGHT-1 (r6 will count down to 0)

lop_y:
	mov   r5, r3         @ var r5 = WIDTH-1 (r5 will count down to 0)

lop_x:
	ldrh  r7, [r1]       @ load tile entry from source
	add   r7, r2         @ add tile base
	strh  r7, [r0]       @ store final tile entry to tilemap

	add   r0, #2         @ increment target ptr
	add   r1, #2         @ increment source ptr

	subs  r5, #1         @ decrement x counter
	bpl   lop_x          @ if x counter >= 0, continue

	sub   r0, r3, lsl #1 @ move target ptr back to one tile past start of row
	sub   r0, #0x42      @ move target ptr back one row and one tile

	subs  r6, #1         @ decrement y counter
	bpl   lop_y          @ if y counter >= 0, continue

	pop   {r4-r7}
	bx    lr

	@ """TSA""" format:
	@ first, 2 byte header, then array of rows

	@ header:
	@ +00 | byte | WIDTH-1
	@ +01 | byte | HEIGHT-1

	@ rows:
	@ each row is an array of WIDTH 2-byte tile entries.
	@ tile entries within rows are arranged left-to-right
	@ rows are in reverse order (bottom-to-top)
	@ there's no padding between rows
