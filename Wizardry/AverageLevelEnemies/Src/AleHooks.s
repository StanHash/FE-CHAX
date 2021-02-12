
	.thumb

	.global AleHook
	.type   AleHook, function

AleHook:
	ldrb  r0, [r6, #0x03]
	lsr   r0, #3

	cmp   r0, #31
	beq   do_ale_thing

	strb  r0, [r5, #0x08]

continue:
	@ replaced
	mov   r1, r5

	@ go back
	ldr   r3, =0x08017D84+1
bxr3:	bx    r3

do_ale_thing:
	ldr   r3, =AleApplyLoadUnitLevel

	mov   r0, r5 @ arg r0 = unit
	mov   r1, r6 @ arg r1 = info

	bl    bxr3

	b     continue
