
#include "gbafe.h"

// TODO: add to CLib
void MakeUnitRescueTransferGraphics(struct Unit* from, struct Unit* to);

extern const u8 gFerryTakePerformId;
extern const u8 gFerryDropPerformId;

extern const u8 gFerryDataTrapId;

extern const u8 gFerryAi1;

static
int FerryIsUnitRescuingAlly(struct Unit* unit)
{
	return (unit->state & US_RESCUING)
		&& (unit->rescueOtherUnit)
		&& (unit->ai1 != gFerryAi1) /* this is to prevent ferrying units to trade units between each-other (as they would likely never get things done that way) */
		&& AreAllegiancesAllied(gActiveUnit->index, unit->index);
}

void FerryAddDropTile(int x, int y)
{
	for (struct Trap* trap = GetTrap(0); trap->type; ++trap)
	{
		if (trap->type == gFerryDataTrapId)
		{
			switch (trap->data[0])
			{

			case 0:
				trap->data[0] = 1;
				trap->data[1] = x;
				trap->data[2] = y;
				return;

			case 1:
				trap->data[0] = 2;
				trap->data[3] = x;
				trap->data[4] = y;
				return;

			case 2:
				continue;

			}
		}
	}

	AddTrap(x, y, gFerryDataTrapId, 0);
}

static
void FerryForEachDropTile(void(*func)(void* user, int x, int y), void* user)
{
	for (struct Trap* trap = GetTrap(0); trap->type; ++trap)
	{
		if (trap->type == gFerryDataTrapId)
		{
			switch (trap->data[0])
			{

			case 2:
				func(user, trap->data[3], trap->data[4]);
				/* fallthrough */
			case 1:
				func(user, trap->data[1], trap->data[2]);
				/* fallthrough */
			case 0:
				func(user, trap->xPosition, trap->yPosition);
				break;

			}
		}
	}
}

static
void FerryCheckDropTile(void* user, int x, int y)
{
	struct Vec2* pos = user;

	if (gMapUnit[y][x] || gMapHidden[y][x])
		return;

	if ((pos->x < 0) && (pos->y < 0))
	{
		pos->x = x;
		pos->y = y;
		return;
	}

	if (RECT_DISTANCE(pos->x, pos->y, gActiveUnit->xPos, gActiveUnit->yPos) > RECT_DISTANCE(x, y, gActiveUnit->xPos, gActiveUnit->yPos))
	{
		pos->x = x;
		pos->y = y;
		return;
	}
}

static
int FerryGetClosestDropTile(struct Vec2* result)
{
	struct Vec2 tmp;

	tmp.x = -1;
	tmp.y = -1;

	FerryForEachDropTile(FerryCheckDropTile, &tmp);

	if ((tmp.x >= 0) && (tmp.y >= 0))
	{
		*result = tmp;
		return TRUE;
	}

	return FALSE;
}

int FerryCheckIsRescuing(unsigned unused)
{
	gAiData.scrResult = ((gActiveUnit->state & US_RESCUING) && (gActiveUnit->rescueOtherUnit));
	return FALSE; /* this never ends ai script */
}

/*
 * For use in Ai scripts: Tries to take a rescued unit from a closeby unit, or move towards a unit rescuing another
 */
int FerryTryTakeInRangeOrMove(unsigned unused)
{
	struct Vec2 target;

	if (AiGetClosestUnitPosition(FerryIsUnitRescuingAlly, &target))
	{
		struct Vec2 adjacent;

		AiFillMovementMapForUnit(gActiveUnit);

		if (GetAiSafestAccessibleAdjacentPosition(target.x, target.y, &adjacent))
			AiSetDecision(adjacent.x, adjacent.y, gFerryTakePerformId, gMapUnit[target.y][target.x], 0, target.x, target.y);
		else
			AiTryMoveTowards(target.x, target.y, AI_DECISION_NONE, 255, FALSE);
	}

	return TRUE; /* this always ends ai script */
}

int FerryTryDropInRangeOrMove(unsigned unused)
{
	struct Vec2 target;

	if (FerryGetClosestDropTile(&target))
	{
		struct Vec2 adjacent;

		AiFillMovementMapForUnit(gActiveUnit);

		if (GetAiSafestAccessibleAdjacentPosition(target.x, target.y, &adjacent))
			AiSetDecision(adjacent.x, adjacent.y, gFerryDropPerformId, gActiveUnit->rescueOtherUnit, 0, target.x, target.y);
		else
			AiTryMoveTowards(target.x, target.y, AI_DECISION_NONE, 255, FALSE);
	}

	return TRUE;
}

CpPerformFunc FerryPerformTake(struct Proc* proc)
{
	struct Unit* from = GetUnit(gAiData.decision.unitTargetIndex);

	gActiveUnit->xPos = gAiData.decision.xMovement;
	gActiveUnit->yPos = gAiData.decision.yMovement;

	MakeUnitRescueTransferGraphics(from, gActiveUnit);
	UnitGive(from, gActiveUnit);

	return NULL;
}

CpPerformFunc FerryPerformDrop(struct Proc* proc)
{
	gActiveUnit->xPos = gAiData.decision.xMovement;
	gActiveUnit->yPos = gAiData.decision.yMovement;

	gActionData.unitActionType = UNIT_ACTION_DROP;

	gActionData.targetIndex = gAiData.decision.unitTargetIndex;
	gActionData.xOther = gAiData.decision.paramB;
	gActionData.yOther = gAiData.decision.paramC;

	ApplyUnitAction(proc);

	return NULL;
}

/*
 * ASMC parameters:
 *   sB = tile position
 */
void ASMC_AddFerryDropTile(struct Proc* proc)
{
	FerryAddDropTile(
		0xFFFF & (gEventSlot[0xB]),
		0xFFFF & (gEventSlot[0xB] >> 16)
	);
}

/*
 * ASMC parameters:
 *   s2 = address of tile list (each tile is a pair of shorts, terminates when x < 0)
 */
void ASMC_AddFerryDropTiles(struct Proc* proc)
{
	struct Vec2* list = (struct Vec2*)(gEventSlot[2]);

	while (list->x >= 0)
	{
		FerryAddDropTile(list->x, list->y);
		++list;
	}
}
