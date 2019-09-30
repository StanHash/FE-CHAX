#include "gbafe.h"

int ActionStaffDoorChestUseItem(Proc*);

typedef int (*ActionFunc) (struct Proc*);

extern const ActionFunc UnitActionCallTable[];

static int RequiresProcYield(ActionFunc func)
{
	const u32 raw = (u32)(func);
	return (raw >> 28) ? 1 : 0;
}

static ActionFunc FilterFunc(ActionFunc func)
{
	const u32 raw           = (u32)(func);
	const ActionFunc result = (ActionFunc)(raw & 0xFFFFFFF);

	return result;
}

int ApplyUnitAction(struct Proc* proc)
{
	gActiveUnit = GetUnit(gActionData.subjectIndex);

#ifndef NO_NIGHTMARE_HARDCODED_CHECK

	if (gActionData.unitActionType == UNIT_ACTION_COMBAT)
	{
		if (GetItemIndex(gActiveUnit->items[gActionData.itemSlotIndex]) == 0xA6)
		{
			ActionStaffDoorChestUseItem(proc);
			return 0;
		}
	}

#endif // NO_NIGHTMARE_HARDCODED_CHECK

	ActionFunc func = UnitActionCallTable[gActionData.unitActionType];

	if (func)
	{
		if (RequiresProcYield(func))
		{
			func = FilterFunc(func);
			func(proc);

			return 0;
		}

		return func(proc);
	}

	return 1;
}

int WaitAction(struct Proc* proc)
{
	gActiveUnit->state |= US_HAS_MOVED;
	return 1;
}
