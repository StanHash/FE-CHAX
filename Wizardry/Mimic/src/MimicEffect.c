#include "Mimic.h"

struct MimicEffectProc {
	PROC_HEADER;

	Unit* pSummonedUnit;
};

static void MECreateSummonUnit(struct MimicEffectProc*);

extern void New6C_SummonGfx_FromActionPos(struct Proc*);

static const struct ProcInstruction sProc_MimicEffect[] = {
	PROC_SET_NAME("Stan:Mimic:Effect"),
	PROC_YIELD,

	PROC_CALL_ROUTINE(MECreateSummonUnit),
	PROC_CALL_ROUTINE(New6C_SummonGfx_FromActionPos),
	PROC_YIELD,

	PROC_END
};

int MimicAction(struct Proc* parent) {
	struct MimicEffectProc* proc = (struct MimicEffectProc*) ProcStartBlocking(sProc_MimicEffect, parent);

	proc->pSummonedUnit = NULL;

	return 0;
}

static void MECreateSummonUnit(struct MimicEffectProc* proc) {
	const struct Unit* subject = GetUnit(gActionData.subjectIndex);
	const struct Unit* toMimic = GetUnit(gActionData.targetIndex);

	struct UnitDefinition eUnit = {
		.charIndex       = MimicCharacterId,
		.classIndex      = toMimic->pClassData->number,
		.leaderCharIndex = subject->pCharacterData->number, // unnecessary/useless but whatever

		.allegiance      = 0,
		.autolevel       = FALSE,
		.level           = toMimic->level,

		.xPosition       = gActionData.xOther,
		.yPosition       = gActionData.yOther,

		.redaCount       = 0,
		.redas           = NULL,

		.items[0] = 0,
		.items[1] = 0,
		.items[2] = 0,
		.items[3] = 0,
	};

	proc->pSummonedUnit = LoadUnit(&eUnit);

	proc->pSummonedUnit->curHP = 1;
	proc->pSummonedUnit->maxHP = toMimic->curHP;

	proc->pSummonedUnit->pow = toMimic->pow;
	proc->pSummonedUnit->skl = toMimic->skl;
	proc->pSummonedUnit->spd = toMimic->spd;
	proc->pSummonedUnit->def = toMimic->def;
	proc->pSummonedUnit->res = toMimic->res;

	proc->pSummonedUnit->lck = 0;

	for (unsigned i = 0; i < ITEM_SLOT_COUNT; ++i)
		proc->pSummonedUnit->items[i] = toMimic->items[i];

	for (unsigned i = 0; i < 8; ++i)
		proc->pSummonedUnit->ranks[i] = toMimic->ranks[i];
}
