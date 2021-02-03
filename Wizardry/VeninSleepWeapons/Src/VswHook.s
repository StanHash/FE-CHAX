
	.thumb

	continue_location = 0x0802B058+1
	return_location = 0x0802B06E+1

	gBattleHitIt = 0x0203A608

	.global VswHook_0802B044
	.type   VswHook_0802B044, function

	@ hook at fe8u:0802B044

VswHook_0802B044:
	@ attacker in r6
	@ defender in r8

	@ load attacker output status

	mov  r0, #0x6F
	ldrb r0, [r6, r0]

	@ compare to sleep, and return false if equal

	cmp r0, #2
	beq return_false

	@ replaced code
	ldr r0, =gBattleHitIt
	ldr r3, [r0]
	ldr r2, [r3]
	lsl r1, r2, #13
	lsr r1, r1, #13
	orr r1, r7
	ldr r0, =0xFFF80000
	and r0, r2
	orr r0, r1
	str r0, [r3]

	ldr r3, =continue_location
	bx r3

return_false:
	mov r0, #0
	ldr r3, =return_location
	bx r3
