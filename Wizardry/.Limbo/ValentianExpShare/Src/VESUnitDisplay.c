#include "VES.h"

static void VESUnitDisplayDrawBase(struct VESUnitDisplayProc* proc);
static void VESUnitDisplayLoop(struct VESUnitDisplayProc* proc);

static const void(*SMS_DisplayOne2)(int, int, int, int, const Unit*) = (void(*)(int, int, int, int, const Unit*))(0x8027E4D);

static const ProcCode sVESUnitDisplayProcScr[] = {
	PROC_SET_NAME(VES_PREFIX":UnitDisplay"),
	PROC_YIELD,

PROC_LABEL(0), // Disabled State
	// PROC_BLOCK,

PROC_LABEL(1), // Enabled State
	PROC_CALL_ROUTINE(VESUnitDisplayDrawBase),
	PROC_LOOP_ROUTINE(VESUnitDisplayLoop),

PROC_LABEL(2), // End
	PROC_END
};

void VESUnitDisplayDrawBase(struct VESUnitDisplayProc* proc) {
	uint16_t* baseTile = gBg1MapBuffer + 4 + 0x20 * (4 + proc->lineId);

	Text_AppendString(&proc->text, String_GetFromIndex(proc->pUnit->pCharacterData->nameTextId));
	Text_Draw(&proc->text, baseTile + 3);
}

void VESUnitDisplayLoop(struct VESUnitDisplayProc* proc) {
	unsigned yBase = 32 + 16 * proc->lineId;

	SMS_DisplayOne2(0, 32, yBase, 0, GetUnit(0x01));
}

struct VESUnitDisplayProc* VESStartUnitDisplay(struct VESProc* proc, unsigned lineId, const Unit* unit) {
	struct VESUnitDisplayProc* newProc = (struct VESUnitDisplayProc*)(StartProc(sVESUnitDisplayProcScr, (Proc*)(proc)));

	newProc->lineId   = lineId;

	newProc->pVESMain = proc;
	newProc->pUnit    = unit;

	Text_InitClear(&newProc->text, 7);

	return newProc;
}
