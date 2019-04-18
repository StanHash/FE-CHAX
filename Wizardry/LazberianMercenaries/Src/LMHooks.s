
	GetPrepScreenUnitListEntry = 0x08095354+1
	_ResetIconGraphics = 0x08003578+1
	LoadNewUiFrameGraphics = 0x0804EB68+1
	LoadObjUiGfx = 0x08015680+1

	.global LMHook0809ACB8
	.type   LMHook0809ACB8, function

LMHook0809ACB8:
	@ status:
	@ {r4-r5, lr} pushed
	@ var r4 = proc

	ldr r3, =GetPrepScreenUnitListEntry

	ldrh r0, [r4, #0x2E]

	bl BXR3

	mov r5, r0

	ldr r3, =LM_PrepScreenHireCheck

	@ implied  @ arg r0 = unit
	mov r1, r4 @ arg r1 = parent proc

	bl BXR3

	cmp r0, #0
	bne goto_not_yet

	@ replaced
	ldr r1, [r5, #0xC]
	mov r0, #2
	lsl r0, #24 @ 0x2000000
	and r0, r1

	ldr r3, =0x0809ACC8+1
	bx r3

goto_not_yet:
	ldr r3, =0x0809AD88+1
BXR3:
	bx r3

@	.global LMHook0809A5F0
@	.type   LMHook0809A5F0, function

@LMHook0809A5F0:
@	ldr r3, =LM_PrepScreenUnitNameDraw
@	mov ip, r3

@	mov r1, #0x1F
@	mov r2, r8
@	and r1, r2
@	lsl r1, r1, #5
@	add r1, #0x10
@	add r1, r9
@	lsl r1, r1, #1
@	ldr r3, =gBg2MapBuffer
@	add r3, r1 @ arg r3 = mapOut

@	mov r0, r4 @ arg r0 = text
@	mov r1, r5 @ arg r1 = unit
@	mov r2, r6 @ arg r2 = color

@	bl BXIP

@	ldr r3, =0x0809A61A+1
@	bx r3

BXIP:
	bx ip

	.global LMHook0809A68C
	.type   LMHook0809A68C, function

LMHook0809A68C:
	mov r0, #1
	and r0, r6
	lsl r4, r0, #3
	sub r4, r4, r0
	lsl r4, r4, #3
	add r4, #0x70
	add r5, #0x18

	ldr r3, =GetPrepScreenUnitListEntry

	mov r0, r6 @ arg r0 = unit

	bl BXR3

	ldr r3, =0x8027B60+1
	mov ip, r3

	mov r3, r0
	mov r1, r4
	mov r2, r5

	push {r1-r3}

	mov r0, #0

	bl BXIP

	pop {r0-r2}

	ldr r3, =LM_PrepScreenUnitSprDraw

	@ implied @ arg r0 = x
	@ implied @ arg r1 = y
	@ implied @ arg r2 = unit

	bl BXR3

	ldr r3, =0x0809A6AC+1
	bx r3

	.global LMHook0809A874
	.type   LMHook0809A874, function

LMHook0809A874:
	push {r4, lr}

	ldr r3, =_ResetIconGraphics
	bl  BXR3

	ldr r3, =LoadNewUiFrameGraphics
	bl  BXR3

	ldr r3, =LoadObjUiGfx
	bl  BXR3

	ldr r3, =LM_LoadCoinGfx
	bl  BXR3

	mov r0, #4

	ldr r3, =0x0809A884+1
	bx r3

	.global LMHook080957D8
	.type   LMHook080957D8, function

LMHook080957D8:
	ldr r3, =LM_ReplaceInitialDeployUnits
	bl  BXR3

	ldr r3, =0x08095838+1
	bx r3
