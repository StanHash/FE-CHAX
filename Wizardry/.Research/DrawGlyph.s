.arm

ARM_DrawTextGlyph:
	stmfd sp!, {r4-r10}

	@ r0 is some table (which one depends on current color id)
	@ r1 is some pointer (to vram)
	@ r2 is pointer to glyph data (+8)
	@ r3 is x cursor mod 8

	mov   r9,  #0xf

	mov   r10, #0x10000
	sub   r10, #1

Continue:
	@ r5 = some offset (depends on local x offset)
	ldr   r4, SomeTextGlyphDrawingLookup
	ldr   r5, [r4, r3, lsl#2]

	@ r4 = current glyph gfx line (?)
	ldr   r4, [r2]

	@ r5:r6 = current glyph line * current x offset offset
	umull r5, r6, r4, r5

	@ I assume glyph lines are 2bpp
	@ This means that there's enough space in a word for 16 pixels
	@ So I assume a glyph line is just a word's worth of 2bpp pixels
	@ However, since we can start from all the way up to pixel #7
	@ We need extra space for the potential 8 extra pixels
	@ Which is probably why this is an umull
	@ The table is 1 -> 4 -> 0x10 -> 0x40 -> ...
	@ because it jumps 2 tiles instead of one (I think?)

	@ r7 = first 4 pixels of line
	mov   r7, r5
	and   r7, r7, #0xff

	@ r7 = Color Line value for the thing?
	ldr   r7, [r0, r7, lsl#1] @ why lsl#1 ?

	@ r8 = Color Line value for pixels 4-7
	mov   r8, r5, lsr#8
	and   r8, r8, #0xff
	ldr   r8, [r0, r8, lsl#1]

	@ r7 = r7 | (r8<<16)
	and   r7, r7, r10
	orr   r7, r7, r8, lsl#16

	@ Applying pixels (tile #0-1)
	ldr   r4, [r1]
	orr   r4, r4, r7
	str   r4, [r1]

	@ r7 = 4bpp pixels 8-11
	mov   r7, r5, lsr#16
	and   r7, r7, #0xff
	ldr   r7, [r0, r7, lsl#1]

	@ r8 = 4bpp pixels 12-15
	mov   r8, r5, lsr#24
	and   r8, r8, #0xff
	ldr   r8, [r0, r8, lsl#1]

	@ r7 = 4bpp pixels 8-15
	and   r7, r7, r10
	orr   r7, r7, r8, lsl#16

	@ Applying pixels 8-15 (tile #2-3)
	ldr   r4, [r1, #0x40]
	orr   r4, r4, r7
	str   r4, [r1, #0x40]

	@ r7 = 4bpp pixels 16-19
	mov   r7, r6
	and   r7, r7, #0xff
	ldr   r7, [r0, r7, lsl#1]

	@ r8 = 4bpp pixels 20-23
	mov   r8, r6, lsr#8
	and   r8, r8, #0xff
	ldr   r8, [r0, r8, lsl#1]

	@ r7 = 4bpp pixels 16-23
	and   r7, r7, r10
	orr   r7, r7, r8, lsl#16

	@ Applying pixels 16-23 (tile #4-5)
	ldr   r4, [r1, #0x80]
	orr   r4, r4, r7
	str   r4, [r1, #0x80]

	@ Next vram 4bpp line
	add   r1, r1, #4

	@ Next glyph 2bpp line
	add   r2, r2, #4

	@ Decrement counter & loop (15 lines?)
	subs  r9, r9, #1
	bpl   Continue

	ldmfd sp!, {r4-r10}
	bx lr

SomeTextGlyphDrawingLookup:
	.long 1
	.long 4
	.long 0x10
	.long 0x40
	.long 0x100
	.long 0x400
	.long 0x1000
	.long 0x4000
