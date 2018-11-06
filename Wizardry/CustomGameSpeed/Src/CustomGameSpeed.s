.thumb

.global CustomGameSpeedHook
.type   CustomGameSpeedHook, %function

GetClassData = (0x08019444+1)

gChapterData = 0x0202BCF0
gKeyStatus   = 0x02024CC0

@ Config struct definitions
Config.value       = 0
Config.checkSlow   = 1
Config.checkAPress = 2

CustomGameSpeedHook:
	@ KNOWN STATE:
	@ r4 = Active MU

	@ Get Class Data

	mov  r0, #0x41 @ MU.displayedClassId
	ldrb r0, [r4, r0]

	ldr  r1, =GetClassData
	bl   BXR1

	@ r4 = Slow Walking option
	ldrb r4, [r0, #7]

	@ r0 = Walking Speed option
	ldr  r0, =gChapterData
	mov  r1, #0x40 @ ChapterData.option1
	ldrb r0, [r0, r1]
	lsr  r0, #7

	@ r0 *= (sizeof(Config) = 4)
	lsl r0, #2

	@ r3 = Current Config data pointer
	adr r3, CGS.ConfigArray
	add r3, r0

	@ r2 = Base speed value
	ldrb r2, [r3, #Config.value]

	@ r0 = Check A Press?
	ldrb r0, [r3, #Config.checkAPress]

	cmp r0, #0
	beq CGS.skipCheckAPress

	ldr  r0, =gKeyStatus
	ldrh r0, [r0, #4] @ Pressed Keys

	mov r1, #1 @ A Button

	@ Test A Button
	tst r0, r1
	beq CGS.skipCheckAPress

	@ Increment speed
	add r2, #1

CGS.skipCheckAPress:
	@ r0 = Check Slow Walking?
	ldrb r0, [r3, #Config.checkSlow]

	cmp r0, #0
	beq CGS.skipSlowWalking

	cmp r4, #0
	beq CGS.skipSlowWalking

	@ Decrement speed
	sub r2, #1

CGS.skipSlowWalking:
	@ return (1 << speedValue)
	mov r0, #1
	lsl r0, r2

	pop {r4}

	pop {r1}
BXR1:	bx r1

.pool
.align

CGS.ConfigArray:
	.byte GameSpeedNormalValue
	.byte GameSpeedNormalCheckSlow
	.byte GameSpeedNormalCheckAPress
	.byte 0 @ pad

	.byte GameSpeedFastValue
	.byte GameSpeedFastCheckSlow
	.byte GameSpeedFastCheckAPress
	.byte 0 @ pad

.align
