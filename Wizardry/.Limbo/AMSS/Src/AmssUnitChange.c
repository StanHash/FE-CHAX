
#include "AmssInternal.h"

struct AmssUnitChangeProc
{
	/* 00 */ PROC_HEADER;

	/* 29 */ s8 direction;
	/* 2C */ struct Unit* unit;
	/* 30 */ unsigned frame;
};

static void AmssUnitChange_OnInit(struct AmssUnitChangeProc* proc);
static void AmssUnitChange_OnOutLoop(struct AmssUnitChangeProc* proc);
static void AmssUnitChange_Change(struct AmssUnitChangeProc* proc);
static void AmssUnitChange_OnInLoop(struct AmssUnitChangeProc* proc);
static void AmssUnitChange_OnEnd(struct AmssUnitChangeProc* proc);

const struct ProcInstruction AmssUnitChangeProcScr[] = {
	PROC_SET_NAME("StanScreen Unit Change"),
	PROC_YIELD,

	PROC_CALL_ROUTINE(AmssUnitChange_OnInit),
	PROC_LOOP_ROUTINE(AmssUnitChange_OnOutLoop),

	PROC_CALL_ROUTINE(AmssUnitChange_Change),
	PROC_LOOP_ROUTINE(AmssUnitChange_OnInLoop),

	PROC_CALL_ROUTINE(AmssUnitChange_OnEnd),

	PROC_END,
};

static void AmssUnitChange_OnInit(struct AmssUnitChangeProc* proc)
{
	gStatScreenData.inTransition = TRUE;

	gLCDIOBuffer.bgControl[0].priority = 1;
	gLCDIOBuffer.bgControl[2].priority = 1;
	gLCDIOBuffer.bgControl[3].priority = 0;

	SetColorEffectsFirstTarget(  FALSE, FALSE, FALSE, TRUE,  FALSE );
	SetColorEffectsSecondTarget( TRUE,  TRUE,  TRUE,  FALSE, TRUE  );

	SetColorEffectBackdropFirstTarget(  TRUE  );
	SetColorEffectBackdropSecondTarget( FALSE );

	proc->frame = 0;
}

static void AmssUnitChange_OnOutLoop(struct AmssUnitChangeProc* proc)
{
	SetColorEffectsParameters(BLEND_EFFECT_ALPHA, proc->frame, 16 - proc->frame, 0);

	gStatScreenData.vOffset = proc->direction * proc->frame;

	if ((proc->frame += 3) >= 16)
		BREAK_PROC(proc);
}

static void AmssUnitChange_Change(struct AmssUnitChangeProc* proc)
{
	AmssChangeUnit(AmssGetMain(), proc->unit);
	proc->frame = 0;
}

static void AmssUnitChange_OnInLoop(struct AmssUnitChangeProc* proc)
{
	SetColorEffectsParameters(BLEND_EFFECT_ALPHA, 16 - proc->frame, proc->frame, 0);

	gStatScreenData.vOffset = proc->direction * (proc->frame - 16);

	if ((proc->frame += 3) >= 16)
		BREAK_PROC(proc);
}

static void AmssUnitChange_OnEnd(struct AmssUnitChangeProc* proc)
{
	gStatScreenData.inTransition = FALSE;

	gStatScreenData.vOffset = 0;
	SetDefaultColorEffects();

	gLCDIOBuffer.bgControl[0].priority = 0;
	gLCDIOBuffer.bgControl[2].priority = 0;
	gLCDIOBuffer.bgControl[3].priority = 3;
}

void AmssStartUnitChange(int direction, struct Unit* next, struct AmssMainProc* parent)
{
	struct AmssUnitChangeProc* proc = START_PROC_BLOCKING(AmssUnitChangeProcScr, parent);

	proc->direction = direction;
	proc->unit = next;

	PlaySfx(0xC8);
}
