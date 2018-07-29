#include "Mimic.h"

extern const struct Unit* gUnitSubject;

static void MimicTryAddToTargetList(struct Unit* unit);

static void MimicUSOnInit(struct TargetSelectionProc*);
static int MimicUSOnSelect(struct TargetSelectionProc*, struct TargetEntry*);
static int MimicUSOnCancel(struct TargetSelectionProc*, struct TargetEntry*);
static void MimicUSOnEnd(struct TargetSelectionProc*);

static void MimicPSOnInit(struct TargetSelectionProc*);
static int MimicPSOnSelect(struct TargetSelectionProc*, struct TargetEntry*);
static int MimicPSOnCancel(struct TargetSelectionProc*, struct TargetEntry*);
static void MimicPSOnEnd(struct TargetSelectionProc*);

static const struct TargetSelectionDefinition sMimicUnitSelection = {
	.onInit       = MimicUSOnInit,
	.onEnd        = MimicUSOnEnd,

	.onInitTarget = NULL,
	.onSwitchIn   = NULL,
	.onSwitchOut  = NULL,

	.onAPress     = MimicUSOnSelect,
	.onBPress     = MimicUSOnCancel,
	.onRPress     = NULL,
};

static const struct TargetSelectionDefinition sMimicPositionSelection = {
	.onInit       = MimicPSOnInit,
	.onEnd        = MimicPSOnEnd,

	.onInitTarget = NULL,
	.onSwitchIn   = NULL,
	.onSwitchOut  = NULL,

	.onAPress     = MimicPSOnSelect,
	.onBPress     = MimicPSOnCancel,
	.onRPress     = NULL,
};

static int CouldUnitBeOnPosition(const struct Unit* unit, int x, int y) {
	if (x < 0 || y < 0)
		return 0; // position out of bounds

	if (x >= gMapSize.width || y >= gMapSize.height)
		return 0; // position out of bounds

	if (gMapUnit[y][x])
		return 0; // a unit is occupying this position

	if (gMapHidden[y][x] & 1)
		return 0; // a hidden unit is occupying this position

	return CanUnitCrossTerrain(unit, gMapTerrain[y][x]);
}

void MimicSetupTargets(const struct Unit* unit) {
	gUnitSubject = unit;

	InitTargets(unit->xPos, unit->yPos);

	ClearMapWith(gMapRange, 0);

	MapAddInRange(unit->xPos, unit->yPos, 5, +1);
	gMapRange[unit->yPos][unit->xPos] = 0;

	ForEachUnitInRange(MimicTryAddToTargetList);
}

static const Vector2 sDirectionOffsetLookup[] = {
	{ -1,  0 }, // Left
	{ +1,  0 }, // Right
	{  0, -1 }, // Up
	{  0, +1 }, // Down
};

void MimicSetupPositionTargets(const struct Unit* unit, const struct Unit* target) {
	gUnitSubject = unit;

	InitTargets(unit->xPos, unit->yPos);
	ClearMapWith(gMapRange, 0);

	for (unsigned dir = 0; dir < 4; ++dir) {
		int x = unit->xPos + sDirectionOffsetLookup[dir].x;
		int y = unit->yPos + sDirectionOffsetLookup[dir].y;

		if (CouldUnitBeOnPosition(target, x, y)) {
			gMapRange[y][x] = 1;
			AddTarget(x, y, 0, 0);
		}
	}
}

static void MimicTryAddToTargetList(struct Unit* unit) {
	if (unit->pCharacterData->affinity)
		return; // non-generic!

	if (UNIT_ATTRIBUTES(unit) & CA_BOSS)
		return; // boss!

	int hasFreeSquare = FALSE;

	for (unsigned dir = 0; dir < 4; ++dir) {
		hasFreeSquare = hasFreeSquare || CouldUnitBeOnPosition(
			unit,

			gActiveUnit->xPos + sDirectionOffsetLookup[dir].x,
			gActiveUnit->yPos + sDirectionOffsetLookup[dir].y
		);
	}

	if (hasFreeSquare)
		AddTarget(unit->xPos, unit->yPos, unit->index, 0);
}

void MimicStartTargetSelection(void) {
	StartTargetSelection(&sMimicUnitSelection);
}

static void MimicUSOnInit(struct TargetSelectionProc* proc) {
	ClearMapWith(gMapMovement, -1);
	DisplayMoveRangeGraphics(MLV_RANGE_RED);

	if (MimicUnitSelectHelpTextId)
		StartBottomHelpText((Proc*)(proc), GetStringFromIndex(MimicUnitSelectHelpTextId));
}

static int MimicUSOnSelect(struct TargetSelectionProc* proc, struct TargetEntry* entry) {
	EndTargetSelection(proc);

	gActionData.targetIndex = entry->unitIndex;

	MimicSetupPositionTargets(gActiveUnit, GetUnit(entry->unitIndex));
	StartTargetSelection(&sMimicPositionSelection);

	return TSE_PLAY_BEEP | TSE_DISABLE;
}

static int MimicUSOnCancel(struct TargetSelectionProc* proc, struct TargetEntry* entry) {
	EndTargetSelection(proc);
	StartProc(gProc_GoBackToUnitMenu, ROOT_PROC_3);

	return TSE_PLAY_BOOP | TSE_DISABLE;
}

static void MimicUSOnEnd(struct TargetSelectionProc* proc) {
	EndBottomHelpText();

	Font_ResetAllocation();

	ClearMapWith(gMapRange, 0);
	HideMoveRangeGraphics();

	FillBgMap(gBg2MapBuffer, 0);
	EnableBgSyncByMask(0b100);

	ClearBG0BG1();
}

static void MimicPSOnInit(struct TargetSelectionProc* proc) {
	ClearMapWith(gMapMovement, -1);
	DisplayMoveRangeGraphics(MLV_RANGE_GREEN);

	if (MimicUnitSelectHelpTextId)
		StartBottomHelpText((Proc*)(proc), GetStringFromIndex(MimicPositionSelectHelpTextId));
}

static int MimicPSOnSelect(struct TargetSelectionProc* proc, struct TargetEntry* entry) {
	EndTargetSelection(proc);

	gActionData.xOther = entry->x;
	gActionData.yOther = entry->y;

	gActionData.actionIndex = MimicCommandActionId;

	return TSE_PLAY_BEEP | TSE_DISABLE;
}

static int MimicPSOnCancel(struct TargetSelectionProc* proc, struct TargetEntry* entry) {
	EndTargetSelection(proc);

	StartProc(gProc_GoBackToUnitMenu, ROOT_PROC_3);

	return TSE_PLAY_BOOP | TSE_DISABLE;
}

static void MimicPSOnEnd(struct TargetSelectionProc* proc) {
	MimicUSOnEnd(proc);
}
