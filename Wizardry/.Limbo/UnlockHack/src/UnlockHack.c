#include "gbafe.h"

int IsThereClosedDoorAt(u8 x, u8 y) __attribute__((long_call));
int IsThereClosedChestAt(u8 x, u8 y) __attribute__((long_call));

static const void(*a)(u8 x, u8 y) = (void(*)(u8, u8))(0x808320C+1);
static const void(*b)(u8 x, u8 y) = (void(*)(u8, u8))(0x80831C8+1);

extern struct BattleUnit gActiveBattleUnit;
extern struct BattleUnit gTargetBattleUnit;

void TryAddPosToUnlockList(u8 x, u8 y) {
	if (IsThereClosedChestAt(x, y))
		AddTarget(x, y, 0, 0);

	else if (IsThereClosedDoorAt(x, y))
		AddTarget(x, y, 0, 0);
}

void UnlockSaveBattleUnitInTarget(struct Proc* proc) {
	// Copy subject battle unit in target battle unit
	// That way, we will be able to restore it later
	// since the item box in the send to convoy menu re-initializes the subject

	memcpy(&gActiveBattleUnit, &gTargetBattleUnit, sizeof(struct BattleUnit));
}

void UnlockTrueEffect(struct Proc* proc) {
	GetUnit(gActionData.subjectIndex)->state |= US_HIDDEN;

	a(gActionData.xOther, gActionData.yOther);
	b(gActionData.xOther, gActionData.yOther);
}

void UnlockRestoreBattleUnit(struct Proc* proc) {
	// Restore subject from target

	memcpy(&gTargetBattleUnit, &gActiveBattleUnit, sizeof(struct BattleUnit));

	// Update items in subject battle unit from the "real" unit
	// the "real" unit is where the event wrote items to

	struct Unit* realUnit = GetUnit(gActiveBattleUnit.unit.index);

	for (unsigned i = 0; i < 5; ++i)
		gActiveBattleUnit.unit.items[i] = realUnit->items[i];
}
