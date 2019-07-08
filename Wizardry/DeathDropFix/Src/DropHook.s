
	.thumb

	.global DropHook
	.type   DropHook, function

DropHook:
	@ KNOWN STATE:
	@   r4 is proc
	@   r5 is unit

	ldrb r0, [r5, #0x0B] @ Unit.id
	mov  r1, #0xC0

	tst r0, r1
	beq finish

	mov r0, r5 @ arg r0 = unit
	bl CheckDeathDrop

	cmp r0, #0
	beq finish

	pop {r4-r6}
	pop {r0}
	bx r0

finish:
	@ replaced

	ldr r3, =ProcStartBlocking

	ldr r0, =gProc_SMSJumpAnimation @ arg r0 = proc scr
	mov r1, r4                      @ arg r1 = parent proc

	bl  BXR3

	ldr r3, =0x08032698+1
	BXR3: bx r3
