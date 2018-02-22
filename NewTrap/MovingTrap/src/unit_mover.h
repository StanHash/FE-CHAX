#ifndef UNIT_MOVER_H
#define UNIT_MOVER_H

#include "gbafe.h"
#include "utility.h"

typedef struct _UnitMoveAnimProc UnitMoveAnimProc;
struct _UnitMoveAnimProc {
	Proc header;
	
	MoveUnitState* pMoveUnit;
	
	Vector2 from;
	Vector2 to;
	
	int clock;
	
	uint8_t locks;
};

UnitMoveAnimProc* NewUnitMoveAnim(MoveUnitState* moveunit, Vector2 from, Vector2 to, Proc* parent);

void UnitMoveAnim_OnDestruct(UnitMoveAnimProc* proc);
void UnitMoveAnim_OnLoop(UnitMoveAnimProc* proc);

#endif // UNIT_MOVER_H
