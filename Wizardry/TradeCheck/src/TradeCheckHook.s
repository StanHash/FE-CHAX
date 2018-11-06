.thumb

@ REPLACED:
@ $08025224

@ LDRB    R0, [R0,#UnitStruct.index]

@ LSLS    R0, R0, #0x18
@ ASRS    R0, R0, #0x18

@ MOVS    R1, #UnitStruct.index
@ LDRSB   R1, [R4,R1]

@ BL      AreAllegiancesEqual

@ LSLS    R0, R0, #0x18

@ CMP     R0, #0
@ BEQ     End

@ LDR     R2, [R5]
@ LDR     R0, [R2,#UnitStruct.pClassData]
@ LDRB    R0, [R0,#ROMClassEntry.number]

@ CMP     R0, #ClassPhantom
@ BEQ     End

@ LDR     R3, [R4,#UnitStruct.pClassData]
@ LDRB    R0, [R3,#ROMClassEntry.number]

@ CMP     R0, #ClassPhantom
@ BEQ     End

Origin   = 0x08025224
End      = . + (0x080252C4 - Origin)
Continue = . + (0x0802524A - Origin)

@ r4 is other
@ r0 is subject

Start:
	mov r1, r4

	ldr r3, =CanUnitTradeWith
	bl  BXR3

	cmp r0, #0
	beq End

	ldr r2, [r5]
	ldr r3, [r4, #4]

	b Continue

BXR3:
	bx r3
