@ This is a cleaned-up version of the FE8 CSA engine by Hextator+circleseverywhere
@ Using more """modern""" methods, references and terminologies.
@ (several hours later) nevermind I'll just rewrite the whole thing because it bad

.thumb

FRAME_DATA_STREAM = 0x0203FFFC

.global CSAStartDim
.global CSAStartNoDim

.type CSAStartDim, %function
.type CSAStartNoDim, %function
.type CSALoop, %function
.type CSAHBlankHandler, %function

@ CSA Proc
@ Referenced as "Callback code" in the original

CSAProc:
	.word 1, CSAProcName
	.word 3, CSALoop
	.word 0, 0

CSAProcName:
	.asciz "CSAEngine"

@ Original engine uses some wierd call table to call external functions because what
@ Let's just use bx r3 like reasonable people do

BXR3:
	bx r3

@ Original ladder for reference:

@ PC_Array:
@ .long 0x0805A16C + 1 @ 0  : GetAISSubjectId
@ .long 0x08055160 + 1 @ 1  : SetSomethingSpellFxToTrue
@ .long 0x08055178 + 1 @ 2  : ClearBG1Position
@ .long 0x08002C7C + 1 @ 3  : StartProc
@ .long 0x0805A310 + 1 @ 4  : GetAISCurrentRoundType
@ .long 0x0805A184 + 1 @ 5  : IsBatteRoundTypeAMiss
@ .long 0x08054FA8 + 1 @ 6  : NewEfxSpellCast
@ .long 0x0805A2B4 + 1 @ 7  : GetCoreAIStruct
@ .long 0x08055278 + 1 @ 8  : StartBattleAnimHitEffectsDefault
@ .long 0x08055424 + 1 @ 9  : <not in fe8u.s>
@ .long 0x08072450 + 1 @ 10 : <not in fe8u.s>
@ .long 0x080533D0 + 1 @ 11 : MoveBattleCameraOnto
@ .long 0x080729A4 + 1 @ 12 : <not in fe8u.s> <sound stuff>
@ .long 0x0805516C + 1 @ 13 : SetSomethingSpellFxToFalse
@ .long 0x08055000 + 1 @ 14 : <not in fe8u.s> <efxSpellCast related>
@ .long 0x08002E94 + 1 @ 15 : BreakProcLoop

CSAStartDim:
	push {r4-r6, lr}

	mov r5, r0

	mov  r0, #0x1
	push {r0}

CSAStart_CommonEntry:
	@ [sp] is 0 for no dim; 1 for dim

	mov r0, r5

	ldr r3, =GetAISSubjectId
	bl  BXR3

	mov r6, r0

	@ r6 is now the boolean "IsRightToLeft"

	mov r0, r5 @ arg r0 = ...?

	ldr r3, =SetSomethingSpellFxToTrue
	bl  BXR3

	ldr r3, =ClearBG1Position
	bl  BXR3

	ldr r0, =CSAProc @ arg r0 = Proc Script
	mov r1, #3       @ arg r1 = Parent (Root Tree #3)

	ldr r3, =StartProc
	bl  BXR3

	mov r4, r0

	@ r4 = CSA Proc

	str r5, [r4, #0x5C] @ csaProc->subject = ais;

	mov  r0, #0
	strh r0, [r4, #0x2C] @ csaProc->timer = 0;

	mov r0, r5 @ arg r0 = AIS

	ldr r3, =GetAISCurrentRoundType
	bl  BXR3

	ldr r3, =IsBatteRoundTypeAMiss
	bl  BXR3

	@ r0 is 1 if current round is a miss

	mov  r1, #0x29
	strb r0, [r4, r1] @ csaProc->isMiss = IsBatteRoundTypeAMiss(GetAISCurrentRoundType(ais));

	ldr r1, =gBattleSpellAnimationId1

	mov r0, r6
	lsl r0, #1

	ldrh r1, [r1, r0]

	@ r1 is spell animation id

	mov r0, #1
	eor r0, r6
	lsl r5, r0, #2

	@ r5 is now 0 for Right-To-Left; 4 for Left-To-Right

	lsl r1, #2      @ r1 = animId * 4
	lsl r6, r1, #2  @ r6 = animId * 16
	add r1, r1, r6  @ r1 = animId * 20 = animId * sizeof(CSAEntry)

	ldr r6, =CSATable
	add r6, r1

	@ r6 is now pointer to the Entry in the CSA table

	cmp r0, #0x0    @
	beq R_T_L

	mov r0, #68

	b L_T_R

R_T_L:
	mov r0, #172

L_T_R:
	ldr  r4, LOW_DUMMY_FRAME_DATA

	add  r4, #0x2C   @
	strb r0, [r4,  #0x02]  @ Low dummy AIS's X offset is set

	add  r4, #0x48   @
	strb r0, [r4,  #0x02]  @ High dummy AIS's X offset is set

	sub r4, #0x74

	mov r0, #0x80
	lsl r0, r0, #0x18

	@ terminate dummy frame datas?

	str r0, [r4, #0x0C]
	str r0, [r4, #0x1C]

	ldr r4, =FRAME_DATA_STREAM
	ldr r0, [r6]
	str r0, [r4] @ [=FRAME_DATA_STREAM] = csaEntry.framedata

	add r5, #4
	ldr r0, [r6, r5]

	@ r0 is pointer to Front OAM

	sub r4, #0x54 @ r4 = high AIS

	str r0, [r4, #0x30] @ highAIS.oam = OAM

	mov  r0, #56
	strb r0, [r4,  #0x0A] @ highAIS.priorty = 56

	add r5, #0x8
	ldr r0, [r6, r5]

	@ r0 is pointer to Back OAM

	sub r4, #0x48 @ r4 = low AIS

	str r0, [r4,  #0x30] @ lowAIS.oam = OAM

	mov  r0, #255
	strb r0, [r4, #0x0A] @ lowAIS.priorty = 255

	pop {r0}

	@ r0 = 0 if no dim else 1

	cmp r0, #0
	beq CSAStart_end

	@ if dim, then start efxSpellCast
	@ which does palette stuff idk

	ldr r3, =NewEfxSpellCast
	bl  BXR3

CSAStart_end:
	pop {r4-r6, pc}

CSAStartNoDim:
	push {r4-r6, lr}

	mov r5, r0

	mov r0, #1
	push {r0}

	b CSAStart_CommonEntry

.align        2
LOW_DUMMY_FRAME_DATA:
.long       0x0203FF34

@ CON_MAP_1_TSA_HEAP_1:
@ .long       0x0203FC00
@ .long       0x02019790 @ This is the buffer that normally gets used. (NOT EVEN NEEDED)

@ HBlank Handler
@ Used by command 53?

CSAHBlankHandler:
	mov  r0, #0x4
	lsl  r0, r0, #0x18

	ldrh r1, [r0, #4]

	@ r1 = [DISPSTAT]

	mov r2, #1

	tst r2, r1
	bne CSAHBlankHandler_end

	ldrh r1, [r0, #6]
	lsr  r2, r1, #1
	neg  r2, r2

	@ r2 = -[VCOUNT]/2

	strh r2, [r0, #0x16]

	@ [BG1VOFS] = -[VCOUNT]/2

CSAHBlankHandler_end:
	bx  lr

@ Main CSA Proc Loop
@ Called "Processor" in the original

CSALoop:
	push {r0-r7, lr}

	mov r4, r0
	ldr r6, [r0, #0x5C]

	mov r0, r6 @ arg r0 = AIS

	ldr r3, =GetCoreAIStruct
	bl  BXR3

	mov r5, r0 @ Puts returned pointer to target unit's AIS into r5

	ldr r7, =FRAME_DATA_STREAM

	ldr r3, [r7] @ r3 points to frame data

NEXT_FRAME_LOOP_POINT:
	ldr r0, [r3] @ r0 is frame data
	lsr r1, r0, #0x18 @ r1 is instruction code

	cmp r1, #0x86
	beq FRAME_TYPE_86_Ladder

	cmp r1, #0x80
	beq TERMINATOR_PASS_ladder

FRAME_TYPE_85:
	lsl r1, r0, #0x18
	lsr r1, r1, #0x18

	@ r1 is command code

	cmp r1, #0x1F
	beq COMMAND_1F

	cmp r1, #0x29
	beq COMMAND_29

	cmp r1, #0x2A
	beq COMMAND_2A

	cmp r1, #0x40
	beq COMMAND_40

	cmp r1, #0x48
	beq COMMAND_48

	cmp r1, #0x1A
	beq COMMAND_1A

	cmp r1, #0x53
	beq COMMAND_53

	cmp r1, #0x08
	bne COMMAND_DEFAULT

COMMAND_08: @ hit effects (hp-stealing)
COMMAND_1A: @ hit effects
	ldrh r0, [r5, #0x10] @ Load target's status flags
	mov  r2, #(1 | 8) @ hit | something else
	orr  r2, r0 @ Set "hit" flags that were immediately loaded
	strh r2, [r5, #0x10] @ Update status flags

	mov r0, r5

	cmp r1, #0x1A
	bne STEAL_HP

	mov  r1, #0x29
	ldrb r1, [r4, r1]

	ldr r3, =StartBattleAnimHitEffectsDefault
	bl  BXR3

	b NO_STEAL_HP

STEAL_HP:
	mov  r1, #0x29
	ldrb r1, [r4, r1]

	ldr r3, =(0x08055424 + 1) @ FIXME (StartBattleAnimResireHitEffects)
	bl  BXR3

NO_STEAL_HP:
	mov  r1, #0x29
	ldrb r1, [r4, r1]

	cmp r1, #1
	beq MISSED_ATTACK

	b COMMAND_COMPLETE

MISSED_ATTACK:
	ldr r0, =FRAME_DATA_STREAM
	ldr r1, [r0]
	ldr r3, =0x80000100 @ TERMINATOR_CODE

MISS_LOOP_START:
	ldr r2, [r1] @data at current frame

	cmp r2, r3
	beq SET_MISS_FRAME

	add r1, #4

	b MISS_LOOP_START

SET_MISS_FRAME:
	str r1, [r0]

	b COMMAND_COMPLETE

COMMAND_1F: @ Play hit sound?
	add  r4, #0x29
	ldrb r1, [r4]

	cmp r1, #0x0 @ Only play hit sound if hitting
	bne COMMAND_COMPLETE

	mov r0, r5

	ldr r3, =(0x08072450 + 1)
	bl  BXR3

	b COMMAND_COMPLETE

COMMAND_29:
	ldr r6, =gLCDIOBuffer

	@ TODO: use the corresponding functions?

	mov  r2, #0x3F
	mov  r1, #0x34
	strb r2, [r6, r1] @ Enable special color effects

	mov r1, #0x3C

	lsl r2, r1, #0x8 @ r2 = 0x3C00
	add r2, #0x42    @ r2 = 0x3C42

	strh r2, [r6, r1] @ LOL optimization; enable alpha blending

	add r1, #0x9 @ r1 = 0x45
	lsr r2, r0, #0x8 @ r2 = [instruction] >> 8
	strb  r2, [r6, r1] @ Set transparency

	sub  r1, #0x1 @ r1 = 0x44

	lsr  r2, r2, #0x8 @ r2 = [instruction] >> 16
	strb r2, [r6, r1] @ Set brightness

	b COMMAND_COMPLETE

COMMAND_2A: @ Enable/Disable bg display?
	ldr r6, =gLCDIOBuffer

	lsl r2, r0, #0x10
	lsr r2, r2, #0x18

	@ r2 = [instruction] >> 8 & 0xFF

	mov r1, #0x1F @ Show map2/map3

	cmp r2, #0x0
	bne DRAW_MAPS_2_AND_3 

	mov r1, #0x13 @ Or you know, don't

DRAW_MAPS_2_AND_3:
	strb  r1, [r6, #0x1]

	b COMMAND_COMPLETE

COMMAND_40: @ Move camera on caster
	mov r0, r6

	mov r1, #1
	neg r1, r1

	ldr r3, =MoveBattleCameraOnto
	bl  BXR3

	b COMMAND_COMPLETE

COMMAND_48: @ play sound/music?
	@ WARNING: This might be wrong

	lsl r0, r0, #0x8  @
	lsr r0, r0, #0x10 @ r0 = music ID to play

	mov  r1, #0x80
	ldrh r2, [r5, #0x2]
	mov  r3, #0x2

	ldr r4, =(0x080729A4 + 1)
	bl  BXR4

	b COMMAND_COMPLETE

BXR4:
	bx r4

FRAME_TYPE_86_Ladder: @ exists because branch was out of range
	b FRAME_TYPE_86

TERMINATOR_PASS_ladder: @ same I guess?
	b TERMINATOR_PASS

COMMAND_53: @ Enable/Disable screen stretch
	lsl r0, r0, #0x8
	lsr r0, r0, #0x10 @ get the param hword

	cmp r0, #0
	bne STRETCHING_ON @C53 for off, C153 for on

STRETCHING_OFF:
	ldr r0, =gLCDIOBuffer

	@ ????? wrong bit?
	@ bit 3 (2^3 = 8) is V-Blank, bit 4 is H-Blank
	@ wait no it just sets only V-Blank nevermind
	@ (still kind of oof)

	mov  r1, #0x8
	strb r1, [r0, #0x4] @ Terminate H blank IRQ stuff (screen stretch)

	@ let's advance to the next frame immediately
	ldr r3, [r7]
	add r3, #0x4
	str r3, [r7]

	b NEXT_FRAME_LOOP_POINT

STRETCHING_ON:
	ldr r4, =gLCDIOBuffer

	mov  r0, #0x18
	strb r0, [r4,  #0x04] @ H-Blank set (part 1)

	mov r4, #0x4
	lsl r4, #0x10
	add r4, #0x2
	lsl r4, #0x8

	@ r4 = 0x4000200 @ IE (Interrupt Enable Register)

	ldrh r0, [r4]
	mov  r1, #2
	orr  r0, r1
	strh r0, [r4] @ H-Blank set (part 2)

	ldr r1, =CSAHBlankHandler

	ldr r3, =SetPrimaryHBlankCallback
	bl  BXR3

	@ let's advance to the next frame immediately
	ldr r3, [r7]
	add r3, #0x4
	str r3, [r7]

	b NEXT_FRAME_LOOP_POINT

COMMAND_DEFAULT:
	cmp r1, #0x13
	ble COMMAND_COMPLETE

	strb r1, [r6, #0x15] @ Send command value

	ldrh r0, [r6, #0x06] @
	add  r0, #0x1
	strh r0, [r6, #0x06] @ Increment delay countdown to ensure it is over 0

	mov r1, #0x1
	neg r1, r1
	lsr r1, r1, #0x14 @ r1 = 0x00000FFF

	ldrh r0, [r6, #0x0C]
	and  r0, r1     @ Clear top bits of the +0x0C flags
	add  r1, #0x1   @ r1 = 0x00001000 ("has commands")
	orr  r0, r1     @ And set the bit for using commands
	strh r0, [r6, #0x0C] @ Update +0x0C flags

	mov  r1, #0x1
	strb r1, [r6, #0x14] @ Set command count

COMMAND_COMPLETE:
	ldr r3, [r7]    @
	add r3, #0x4    @
	str r3, [r7]    @

COMMAND_ESCAPE:
	b Exit

TERMINATOR_PASS:
	b FRAME_TYPE_80

FRAME_TYPE_86:
	@ FrameID--
	ldrh r2, [r4, #0x2C]
	sub  r2, #1
	strh r2, [r4, #0x2C]

	mov r3, #0x0    @

	cmp r2, #0x0
	bgt NO_ADVANCE
	beq ADVANCE

	strh  r0, [r4, #0x2C]  @Update frame ID

	b NO_ADVANCE

ADVANCE:
	mov r3, #1

NO_ADVANCE:
	push {r3} @ r3 is boolean of whether to advance

	mov r4, r7
	sub r4, #0xC8 @ r4 = LOW_DUMMY_FRAME_DATA

	@ Write current frame to dummy frame datas
	str r0, [r4]
	str r0, [r4, #0x10]

	ldr r3, [r7] @ r3 is now the current frame data pointer
	ldr r2, [r3, #0x1C] @ this is the tsa pointer.

	@ hang on, we need to flip the tsa using 8055670 (r0 and r1 are separate tsas. just put the same in both)
	@ think they write to 2019790. TODO: find out if this is automatically updated.
	mov r1, r2

	mov r0, r6 @ oh god i sure hope this works

	ldr r3, =(0x08055670 + 1) @ FIXME
	bl  BXR3

	@ no need to copy haha we're using the proper buffer now

	ldr r3, [r7] @ r3 is now the current frame data pointer again
	ldr r0, [r3, #0x04] @ Get OAM sheet pointer

	cmp r0, #0 @ Check if it's null
	bne VALID_OAM_SHEET

	strh r0, [r1] @ Set fill value
	mov  r0, r1   @ Set source address

	mov r2, #0x1  @ Set fill bit
	lsl r2, #0x13
	add r2, #0x40 @ Set word count
	lsl r2, #0x5  @ Finish shifting

	swi #0x0B     @ CpuSet

	b INVALID_OAM_SHEET

VALID_OAM_SHEET:
	@ r0 contains graphics data

	mov r1, #0x80
	lsl r1, #5 @r1 = 0x00001000

	ldr r3, =SomeImageStoringRoutine_SpellAnim
	bl  BXR3

INVALID_OAM_SHEET:
	@ Fill dummy AIS structs and set parent AIS pointer
	@ Also complete filling of dummy frame data

	ldr r3, [r7]
	ldr r2, [r3, #0x08] @ r2 = OAM data offset
	ldr r1, [r3, #0x0C] @ r1 = BG OAM data offset

	str r1, [r4, #0x08]
	str r2, [r4, #0x18]
	str r4, [r4, #0x4C] @ Mind blowing optimization (Stan's note: because the AIS starts at +2C, this is AIS+20 aka pointer to current frame)

	add r4, #0x10
	mov r3, r4    @ r3 = FrameData2
	add r4, #0x84
	str r3, [r4]  @ [AIS2+20] = FrameData2

	mov r4, r3
	add r4, #0x10 @ r4 = DEFAULT_OAM_PTR (Stan's note: thank you; this could have helped the 20 previous times too)

	mov  r0, #0x1
	strb r0, [r4]
	strb r0, [r4, #0x0C]

	add  r4, #0x54 @ r4 = HI_DUMMY_AIS_PTR
	strb r0, [r4]  @ AIS+00 is main state bits
	strb r0, [r4, #0x06]  @ AIS+06 is deley

	sub  r4, #0x48 @ r4 = LOW_DUMMY_AIS_PTR
	strb r0, [r4, #0x06] @ AIS+06 is deley

	add  r4, #0x48 @ r4 = HI_DUMMY_AIS_PTR
	ldrh r0, [r6, #0x04]
	strh r0, [r4, #0x04] @ Sync y with subject AIS?

	sub  r4, #0x48 @ r4 = LOW_DUMMY_AIS_PTR
	strh r0, [r4, #0x04] @ Same

	mov  r0, #0x28 @ Sets palette/priority to use for sprites (pal 2; some priority)
	lsl  r0, #0x8
	add  r0, #0x40 @ Sets tile base to use for sprites (tile 0x40)
	strh r0, [r4, #0x08] @ TODO (Stan): add this to doc because I forgot (AIS.oam2base)

	add  r4, #0x48 @ r4 = HI_DUMMY_AIS_PTR
	mov  r0, #0x24
	lsl  r0, #0x8
	add  r0, #0x40
	strh r0, [r4, #0x08] @ +0x08 of dummy AISs set

	sub r4, #0x48 @ r4 = LOW_DUMMY_AIS_PTR

	@ ew ew ew ew I found your bug Snakey
	@ linking AISs into the array without those AISs being into the array is bad
	@ Because next time the array will be re-sorted (in case of a priority change, like say with a front dodge)
	@ The links will be all messed up

	str r5, [r4, #0x34] @ set previous AIS of LOW_DUMMY_AIS_PTR to main AIS
	ldr r0, [r5, #0x38]

	cmp r0, r4
	beq SKIP_CHAIN_1

	str r0, [r4, #0x38] @ +0x38 of low dummy AIS set; low AIS complete
	str r4, [r0, #0x34] @ Low dummy AIS injected completely

SKIP_CHAIN_1:
	str r4, [r5, #0x38] @ Low dummy AIS is now child of defender
	add r4, #0x48 @ r4 = HI_DUMMY_AIS_PTR

	ldr r1, =gpAISFirst
	ldr r6, [r1]

	ldr r3, [r6, #0x38]
	str r4, [r6, #0x38] @ Dummy is child of parent

	cmp r3, r4
	beq SKIP_CHAIN_2

	str r3, [r4, #0x38]  @Child is child of dummy

SKIP_CHAIN_2:
	str r6, [r4, #0x34]  @ Parent is parent of dummy

	ldr r3, [r7]
	ldr r1, =0x02017790 @ FIXME
	ldr r0, [r3, #0x10] @ Get BG sheet pointer

	cmp r0, #0x0 @ Check if it's null
	bne VALID_BG_SHEET_1

	strh r0, [r1]    @Set fill value
	mov r0, r1    @Set source address
	mov r2, #0x1    @Set fill bit
	lsl r2, r2, #0x13 @
	add r2, #0x80   @Set word count
	lsl r2, r2, #0x5  @Finish shifting
	swi #0x0B     @CpuSet

	b INVALID_BG_SHEET_1

VALID_BG_SHEET_1:
	push {r3}

	mov r1, #0x20
	lsl r1, #8 @ r1 = 0x2000

	ldr r3, =SomeImageStoringRoutine_SpellAnim2
	bl  BXR3

	pop {r3}

INVALID_BG_SHEET_1:
	ldr r3, [r7]
	ldr r0, [r3, #0x18] @ r0 = BG palette pointer

	cmp r0, #0x0    @Check if it's null
	beq NO_BG_PALETTE

	ldr r1, =gPalette1Buffer
	mov r2, #0x8    @8 words in a palette

	swi #0x0C     @CpuFastSet

	ldr r3, [r7]

NO_BG_PALETTE:
	ldr r0, [r3, #0x14] @ r0 = OAM palette pointer

	cmp r0, #0x0
	beq NO_OAM_PALETTE

	ldr r1, =gPalette12Buffer
	mov r2, #0x8 @ 8 words in a palette

	swi #0x0C @ CpuFastSet

NO_OAM_PALETTE:
	pop {r0}

	cmp r0, #0x0 @ r0 is boolean of whether to advance to next frame
	beq Exit

	ldr r3, [r7]
	@ r3 = pointer to next frame
	add r3, #0x20 @ 1c is now the TSA pointer. ain't i a cool one.
	str r3, [r7]  @ Update frame data stream pointer

	b Exit

FRAME_TYPE_80:
	add r3, #0x4
	str r3, [r7]

	ldrh  r2, [r4, #0x2C]

	@ (Stan's note: Suddenly compare optimization)

	sub r2, #1
	bmi NEGATIVE_DELAY

	strh r2, [r4, #0x2C]

NEGATIVE_DELAY:
	cmp r2, #0 @ For checking if frame delay is <= 0
	bgt Exit

	lsl r1, r0, #0x10
	lsr r1, r1, #0x18

	cmp r1, #0x0
	beq TERMINATE_ANIMATION

	add  r4, #0x29
	ldrb r1, [r4] @ that's the miss bool I think

	sub r4, #0x29

	cmp r1, #0
	beq Exit

TERMINATE_ANIMATION:
	mov  r2, #0x0
	ldr  r1, =FRAME_DATA_STREAM
	sub  r1, #0x9C   @
	strb r2, [r1]    @

	add  r1, #0x48   @
	strb r2, [r1]    @ Dummy AISs are disabled

	@ clear both the tsa and the buffer. this is important for missed animations
	ldr  r0, =0x020234A8 @ FIXME
	mov  r1, #0x0    @
	strh r1, [r0]    @

	mov r1, r0
	mov r2, #0x1
	lsl r2, #0x15 @
	add r2, #0x52   @
	lsl r2, #0x3  @
	swi #0x0B @ CpuSet; clears TSA buffer

	ldr  r0, =0x06006800 @ FIXME
	mov  r1, #0x0
	strh r1, [r0]

	mov r1, r0
	mov r2, #0x1
	lsl r2, r2, #0x15
	add r2, #0x52
	lsl r2, r2, #0x3
	swi #0x0B @ CpuSet; clears TSA

	@ should i clear the stuff here too?

	ldr  r0, =FRAME_DATA_STREAM
	sub  r0, #0xA8   @ r0 points to dummy OAM data
	mov  r1, #0x1
	strb r1, [r0]
	str  r0, [r0, #0x48] @ OAM of low AIS dummy is terminated
	mov  r1, #0x84
	str  r0, [r0, r1] @ OAM of high AIS dummy is terminated

	ldr r3, =SetSomethingSpellFxToFalse
	bl  BXR3

	ldr r3, =(0x08055000 + 1) @ FIXME
	bl  BXR3

	mov r0, r4

	ldr r3, =BreakProcLoop
	bl  BXR3

Exit:
	pop {r0-r7, pc} @ Hurf (Stan's note: ?)
