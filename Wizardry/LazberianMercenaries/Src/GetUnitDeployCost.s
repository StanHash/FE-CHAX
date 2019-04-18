
	.thumb

GetUnitDeployCost:
	@ arg r0 = unit

	@ this currently returns
	@ unit->pow*4 + unit->res*4 + unit->spd*4 + unit->def*4 + unit->hpMax*2 + unit->lck*2 + unit->skl*2

	@ feel free to edit as you see fit

	mov  r1, #0x12
	ldsb r1, [r0, r1] @ r1 = unit->hpMax
	lsl  r2, r1, #1   @ result = unit->hpMax*2

	mov  r1, #0x14
	ldsb r1, [r0, r1] @ r1 = unit->str
	lsl  r1, #2       @ r1 = unit->str*4
	add  r2, r1       @ result += unit->str*4

	mov  r1, #0x15
	ldsb r1, [r0, r1] @ r1 = unit->skl
	lsl  r1, #1       @ r1 = unit->skl*2
	add  r2, r1       @ result += unit->skl*2

	mov  r1, #0x16
	ldsb r1, [r0, r1] @ r1 = unit->spd
	lsl  r1, #2       @ r1 = unit->spd*4
	add  r2, r1       @ result += unit->spd*4

	mov  r1, #0x17
	ldsb r1, [r0, r1] @ r1 = unit->def
	lsl  r1, #2       @ r1 = unit->def*4
	add  r2, r1       @ result += unit->def*4

	mov  r1, #0x18
	ldsb r1, [r0, r1] @ r1 = unit->res
	lsl  r1, #2       @ r1 = unit->res*4
	add  r2, r1       @ result += unit->res*4

	mov  r1, #0x19
	ldsb r1, [r0, r1] @ r1 = unit->lck
	lsl  r1, #1       @ r1 = unit->lck*2
	add  r2, r1       @ result += unit->lck*2

	mov r0, r2 @ result r0 = result
	bx  lr     @ return

	.pool
	.align
