.thumb

.global SumStaffUsability
.type   SumStaffUsability, %function

.global SumStaffEffect
.type   SumStaffEffect, %function

SumStaffUsability:
	mov r0, #1

	ldr r3, =(0x08028C06+1)
	bx  r3

.pool
.align

SumStaffEffect:
	bl SumStartPosSelection

	ldr r3, =(0x08029062+1)
	bx  r3
