
	.thumb

	.global FcukTanksHook
	.type   FcukTanksHook, function

FcukTanksHook:
	ldr r3, =gBattleActor

	mov  r0, #0x5A
	ldsh r0, [r3, r0]

	ldr r3, =gBattleTarget

	mov  r1, #0x5C
	ldsh r1, [r3, r1]

	sub r1, r0, r1 @ r1 = attack - defense = damage per hit

	mov r0, #0

	cmp r1, #0
	bgt end

	mov r0, #1

end:
	ldr r3, =0x0803DF18+1

	cmp r0, #0

BXR3:
	bx r3
