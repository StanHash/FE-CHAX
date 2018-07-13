#include "gbafe.h"

extern int gEventSlot[];

// FIXME
static const Vector2* const pCameraDisplayPosition = (const Vector2*) (0x0202BCB0 + 0x0C);

static int sign(int value) { return (value>>31) - (-value>>31); }

static int CanUnitBeOnPosition(Unit* unit, int x, int y) {
	if (x < 0 || y < 0)
		return 0; // position out of bounds

	if (x >= gMapSize.x || y >= gMapSize.y)
		return 0; // position out of bounds

	if (gMapUnit[y][x])
		return 0; // a unit is occupying this position

	if (gMapHidden[y][x] & 1)
		return 0; // a hidden unit is occupying this position

	return CanUnitCrossTerrain(unit, gMapTerrain[y][x]);
}

static void ApplyListedMapChanges(const short* list) {
	for (; list[0] > 0; list += 2) {
		int mapChangeId = list[0];
		int action      = list[1];

		if (action == 1) {
			// Trigger map change

			if (AreMapChangeTriggered(mapChangeId))
				continue;

			AddMapChange(mapChangeId);
			ApplyMapChangesById(mapChangeId);
		} else if (action == 0) {
			// Revert map change

			if (!AreMapChangeTriggered(mapChangeId))
				continue;

			RemoveMapChange(mapChangeId);
			RevertMapChangesById(mapChangeId);
		}
	}
}

void MapChangeListASMC(Proc* proc) {
	InitMapChangeGraphics(); // copy bg3 to bg2 (for fade)

	ApplyListedMapChanges((const short*)(gEventSlot[2]));

	RefreshTileMaps();
	UpdateUnitsUnderRoof();

	DrawTileGraphics(); // update bg3
	StartBlockingBMXFADE(1, proc); // fade from bg2 to bg3
}

void TorchStaffAnimASMC(Proc* proc) {
	static const ProcCode* procCode = (const ProcCode*)(0x89A3C24);

	struct TorchAnimProc {
		PROC_HEADER;

		/* 2C */ Unit* unit; // unused?

		/* 30 */ int xPos;
		/* 34 */ int yPos;
	};

	struct TorchAnimProc* newProc = (struct TorchAnimProc*)(StartBlockingProc(procCode, proc));

	newProc->xPos = ((gEventSlot[0xB] & 0xFFFF) * 16) - pCameraDisplayPosition->x;
	newProc->yPos = ((gEventSlot[0xB] >> 16) * 16)    - pCameraDisplayPosition->y;
}

void TorchSetASMC(Proc* proc) {
	int x = (gEventSlot[0xB] & 0xFFFF);
	int y = (gEventSlot[0xB] >> 16);

	AddTrap(x, y, 10, 8);

	InitMapChangeGraphics(); // copy bg3 to bg2 (for fade)

	RefreshEntityMaps();
	RefreshTileMaps();
	DrawTileGraphics();

	StartBlockingBMXFADE(1, proc); // fade from bg2 to bg3
}

struct UnitSlideAnimProc {
	PROC_HEADER;

	Unit* unit;

	short xNow;
	short yNow;

	short xTarget;
	short yTarget;
};

static void USAOnLoop(struct UnitSlideAnimProc*);
static void USAOnInit(struct UnitSlideAnimProc*);
static void USAOnEnd(struct UnitSlideAnimProc*);

static const ProcCode sProc_UnitSlideAnim[] = {
	PROC_SET_NAME("Stan:CCBlitzCh27:UnitSlideAnim"),
	PROC_SET_DESTRUCTOR(USAOnEnd),

	PROC_YIELD,

	PROC_CALL_ROUTINE(USAOnInit),
	PROC_LOOP_ROUTINE(USAOnLoop),

	PROC_END
};

struct UnitSlideAnimProc* StartUnitSlideAnim(Unit* unit, int xTarget, int yTarget, Proc* parent) {
	struct UnitSlideAnimProc* result;

	if (parent)
		result = (struct UnitSlideAnimProc*) StartBlockingProc(sProc_UnitSlideAnim, parent);
	else
		result = (struct UnitSlideAnimProc*) StartProc(sProc_UnitSlideAnim, ROOT_PROC_3);

	result->unit    = unit;

	result->xNow    = unit->xPos * 16;
	result->yNow    = unit->yPos * 16;

	result->xTarget = xTarget * 16;
	result->yTarget = yTarget * 16;

	EnsureCameraOntoPosition((Proc*)(result), unit->xPos, unit->yPos);

	return result;
}

void USAOnInit(struct UnitSlideAnimProc* proc) {
	HideUnitSMS(proc->unit);
}

void USAOnLoop(struct UnitSlideAnimProc* proc) {
	static const void(*DrawUnitSMS)(int, int, int, Unit*) = (void(*)(int, int, int, Unit*))(0x8027B60+1);

	proc->xNow += sign(proc->xTarget - proc->xNow) * 2 * (1 + gChapterData.gameSpeedOption);
	proc->yNow += sign(proc->yTarget - proc->yNow) * 2 * (1 + gChapterData.gameSpeedOption);

	DrawUnitSMS(
		7,
		proc->xNow - pCameraDisplayPosition->x,
		proc->yNow - pCameraDisplayPosition->y,
		proc->unit
	);

	if (proc->xNow == proc->xTarget && proc->yNow == proc->yTarget)
		BreakProcLoop((Proc*)(proc));
}

void USAOnEnd(struct UnitSlideAnimProc* proc) {
	ShowUnitSMS(proc->unit);

	RefreshEntityMaps();
	SMS_UpdateFromGameData();
}

struct UnitSlideEffectProc {
	PROC_HEADER;

	unsigned unitIndex;
};

static void USEOnInit(struct UnitSlideEffectProc*);
static void USEOnLoop(struct UnitSlideEffectProc*);

static const ProcCode sProc_UnitSlideEffect[] = {
	PROC_SET_NAME("Stan:CCBlitzCh27:UnitSlideEffect"),

	PROC_CALL_ROUTINE(USEOnInit),
	PROC_LOOP_ROUTINE(USEOnLoop),
	PROC_END
};

void SlideAllUnitsASMC(Proc* proc) {
	StartBlockingProc(sProc_UnitSlideEffect, proc);
}

void USEOnInit(struct UnitSlideEffectProc* proc) {
	proc->unitIndex = 0;
}

enum {
	DIR_NONE,

	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
};

enum {
	DIR_MAP_WIDTH  = 5,
	DIR_MAP_HEIGHT = 5,
};

// Clockwise rotation
static const u8 sDirectionMapLookup[] = {
	DIR_RIGHT, DIR_RIGHT, DIR_RIGHT, DIR_RIGHT, DIR_DOWN,
	DIR_UP,    DIR_RIGHT, DIR_RIGHT, DIR_DOWN,  DIR_DOWN,
	DIR_UP,    DIR_UP,    DIR_NONE,  DIR_DOWN,  DIR_DOWN,
	DIR_UP,    DIR_UP,    DIR_LEFT,  DIR_LEFT,  DIR_DOWN,
	DIR_UP,    DIR_LEFT,  DIR_LEFT,  DIR_LEFT,  DIR_LEFT,
};

static const Vector2 sDirectionOffsetLookup[] = {
	{  0,  0 }, // DIR_NONE

	{ -1,  0 }, // DIR_LEFT
	{ +1,  0 }, // DIR_RIGHT
	{  0, -1 }, // DIR_UP
	{  0, +1 }, // DIR_DOWN
};

void USEOnLoop(struct UnitSlideEffectProc* proc) {
	proc->unitIndex++;

	for (; proc->unitIndex < 0x100; proc->unitIndex++) {
		Unit* unit = GetUnit(proc->unitIndex);

		if (unit && unit->pCharacterData && !(unit->state & (US_RESCUED | US_NOT_DEPLOYED | US_DEAD | 0x00010000))) {
			int xGrid = Div(unit->xPos * DIR_MAP_WIDTH,  gMapSize.x);
			int yGrid = Div(unit->yPos * DIR_MAP_HEIGHT, gMapSize.y);

			int direction = sDirectionMapLookup[xGrid + DIR_MAP_WIDTH * yGrid];

			if (direction == DIR_NONE)
				continue;

			int xNew = unit->xPos + sDirectionOffsetLookup[direction].x;
			int yNew = unit->yPos + sDirectionOffsetLookup[direction].y;

			if (!CanUnitBeOnPosition(unit, xNew, yNew))
				continue;

			if (gMapFog[yNew][xNew] || gMapFog[unit->yPos][unit->xPos]) {
				StartUnitSlideAnim(unit, xNew, yNew, (Proc*)(proc));

				unit->xPos = xNew;
				unit->yPos = yNew;

				return;
			} else {
				unit->xPos = xNew;
				unit->yPos = yNew;

				continue;
			}
		}
	}

	BreakProcLoop((Proc*)(proc));
}
